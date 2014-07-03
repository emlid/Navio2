/*
Mahony AHRS algorithm implemented by Madgwick
See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms

Adapted by Igor Vereninov (igor.vereninov@emlid.com)
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com
*/

#ifndef AHRS_HPP
#define AHRS_HPP

#include <cmath>
#include <stdio.h>

class AHRS{
private:
	float q0, q1, q2, q3;
	float gyroOffset[3];
	float twoKi;
	float twoKp;
	float integralFBx, integralFBy, integralFBz;
public:
	AHRS(float q0 = 1, float q1 = 0, float q2 = 0, float q3 = 0)
	:	 q0(q0), q1(q1), q2(q2), q3(q3), twoKi(0), twoKp(2)  {;}

	void update(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float dt)
	{
		float recipNorm;
	    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
		float hx, hy, bx, bz;
		float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
		float halfex, halfey, halfez;
		float qa, qb, qc;

		// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
		if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
			updateIMU(gx, gy, gz, ax, ay, az, dt);
			return;
		}

		// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
		if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

			// Normalise accelerometer measurement
			recipNorm = invSqrt(ax * ax + ay * ay + az * az);
			ax *= recipNorm;
			ay *= recipNorm;
			az *= recipNorm;

			// Normalise magnetometer measurement
			recipNorm = invSqrt(mx * mx + my * my + mz * mz);
			mx *= recipNorm;
			my *= recipNorm;
			mz *= recipNorm;

	        // Auxiliary variables to avoid repeated arithmetic
	        q0q0 = q0 * q0;
	        q0q1 = q0 * q1;
	        q0q2 = q0 * q2;
	        q0q3 = q0 * q3;
	        q1q1 = q1 * q1;
	        q1q2 = q1 * q2;
	        q1q3 = q1 * q3;
	        q2q2 = q2 * q2;
	        q2q3 = q2 * q3;
	        q3q3 = q3 * q3;

	        // Reference direction of Earth's magnetic field
	        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
	        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
	        bx = sqrt(hx * hx + hy * hy);
	        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

			// Estimated direction of gravity and magnetic field
			halfvx = q1q3 - q0q2;
			halfvy = q0q1 + q2q3;
			halfvz = q0q0 - 0.5f + q3q3;
	        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
	        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
	        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

			// Error is sum of cross product between estimated direction and measured direction of field vectors
			halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
			halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
			halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

			// Compute and apply integral feedback if enabled
			if(twoKi > 0.0f) {
				integralFBx += twoKi * halfex * dt;	// integral error scaled by Ki
				integralFBy += twoKi * halfey * dt;
				integralFBz += twoKi * halfez * dt;
				gx += integralFBx;	// apply integral feedback
				gy += integralFBy;
				gz += integralFBz;
			}
			else {
				integralFBx = 0.0f;	// prevent integral windup
				integralFBy = 0.0f;
				integralFBz = 0.0f;
			}

			// Apply proportional feedback
			gx += twoKp * halfex;
			gy += twoKp * halfey;
			gz += twoKp * halfez;
		}

		// Integrate rate of change of quaternion
		gx *= (0.5f * dt);		// pre-multiply common factors
		gy *= (0.5f * dt);
		gz *= (0.5f * dt);
		qa = q0;
		qb = q1;
		qc = q2;
		q0 += (-qb * gx - qc * gy - q3 * gz);
		q1 += (qa * gx + qc * gz - q3 * gy);
		q2 += (qa * gy - qb * gz + q3 * gx);
		q3 += (qa * gz + qb * gy - qc * gx);

		// Normalise quaternion
		recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
		q0 *= recipNorm;
		q1 *= recipNorm;
		q2 *= recipNorm;
		q3 *= recipNorm;
	}

	void updateIMU(float ax, float ay, float az, float gx, float gy, float gz, float dt)
	{
		float recipNorm;
		float halfvx, halfvy, halfvz;
		float halfex, halfey, halfez;
		float qa, qb, qc;

		gx -= gyroOffset[0];
		gy -= gyroOffset[1];
		gz -= gyroOffset[2];

		// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
		if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

			// Normalise accelerometer measurement
			recipNorm = invSqrt(ax * ax + ay * ay + az * az);
			ax *= recipNorm;
			ay *= recipNorm;
			az *= recipNorm;

			// Estimated direction of gravity and vector perpendicular to magnetic flux
			halfvx = q1 * q3 - q0 * q2;
			halfvy = q0 * q1 + q2 * q3;
			halfvz = q0 * q0 - 0.5f + q3 * q3;

			// Error is sum of cross product between estimated and measured direction of gravity
			halfex = (ay * halfvz - az * halfvy);
			halfey = (az * halfvx - ax * halfvz);
			halfez = (ax * halfvy - ay * halfvx);

			// Compute and apply integral feedback if enabled
			if(twoKi > 0.0f) {
				integralFBx += twoKi * halfex * dt;	// integral error scaled by Ki
				integralFBy += twoKi * halfey * dt;
				integralFBz += twoKi * halfez * dt;
				gx += integralFBx;	// apply integral feedback
				gy += integralFBy;
				gz += integralFBz;
			}
			else {
				integralFBx = 0.0f;	// prevent integral windup
				integralFBy = 0.0f;
				integralFBz = 0.0f;
			}

			// Apply proportional feedback
			gx += twoKp * halfex;
			gy += twoKp * halfey;
			gz += twoKp * halfez;
		}

		// Integrate rate of change of quaternion
		gx *= (0.5f * dt);		// pre-multiply common factors
		gy *= (0.5f * dt);
		gz *= (0.5f * dt);
		qa = q0;
		qb = q1;
		qc = q2;
		q0 += (-qb * gx - qc * gy - q3 * gz);
		q1 += (qa * gx + qc * gz - q3 * gy);
		q2 += (qa * gy - qb * gz + q3 * gx);
		q3 += (qa * gz + qb * gy - qc * gx);

		// Normalise quaternion
		recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
		q0 *= recipNorm;
		q1 *= recipNorm;
		q2 *= recipNorm;
		q3 *= recipNorm;
	}

	void setGyroOffset(float offsetX, float offsetY, float offsetZ)
	{
		gyroOffset[0] = offsetX;
		gyroOffset[1] = offsetY;
		gyroOffset[2] = offsetZ;
	}

	void getEuler(float* roll, float* pitch, float* yaw)
	{
	   *roll = atan2(2*(q0*q1+q2*q3), 1-2*(q1*q1+q2*q2)) * 180.0/M_PI;
	   *pitch = asin(2*(q0*q2-q3*q1)) * 180.0/M_PI;
	   *yaw = atan2(2*(q0*q3+q1*q2), 1-2*(q2*q2+q3*q3)) * 180.0/M_PI;
	}

	float invSqrt(float x)
	{
		float halfx = 0.5f * x;
		float y = x;
		long i = *(long*)&y;
		i = 0x5f3759df - (i>>1);
		y = *(float*)&i;
		y = y * (1.5f - (halfx * y * y));
		return y;
	}
	float getW()
	{
		return  q0;
	}
	float getX()
	{
		return  q1;
	}
	float getY()
	{
		return  q2;
	}
	float getZ()
	{
		return  q3;
	}

};

#endif // AHRS_hpp
