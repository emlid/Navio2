#!/bin/bash

set -ex

ROS_RELEASE=$(rosversion -d)
DEB_RELEASE=$(lsb_release -sc)

CFG_NAME="${ROS_RELEASE}.rosinstall"
INSTALL_PATH="/opt/ros/${ROS_RELEASE}"
WORKSPACE_NAME="src"
ROS_BUILD_PATH="/tmp/ros/"
PYTHON_PATH="/usr/bin/python3"

ROS_PKGS="$1"

if [ -z "${ROS_PKGS}" ]; then
    echo "Pass the string with needed ROS packages"
    exit 1
fi

sudo apt-get update
pip install -U future

sudo apt -y install --fix-missing python3-catkin-pkg python3-rosdistro python3-rosinstall-generator python3-rosdep

if [[ ! -d "/etc/ros" ]]; then
    c_rehash /etc/ssl/certs
    rosdep init
fi

rosdep update

if [[ ! -d "${ROS_BUILD_PATH}" ]]; then
    mkdir "${ROS_BUILD_PATH}"
fi

cd "${ROS_BUILD_PATH}"

rosinstall_generator "${ROS_PKGS}" --rosdistro "${ROS_RELEASE}" --deps --wet-only --tar | tee "${CFG_NAME}"
wstool init "${WORKSPACE_NAME}" "${CFG_NAME}" || wstool merge -t "${WORKSPACE_NAME}" "${CFG_NAME}"

wstool update -t "${WORKSPACE_NAME}"
rosdep install -y --from-paths "${WORKSPACE_NAME}" --ignore-src --rosdistro "${ROS_RELEASE}" -r --os=debian:"${DEB_RELEASE}"

sudo ./"${WORKSPACE_NAME}"/catkin/bin/catkin_make_isolated --install -DPYTHON_EXECUTABLE="${PYTHON_PATH}" -DCMAKE_BUILD_TYPE=Release --install-space "${INSTALL_PATH}" -DCATKIN_ENABLE_TESTING=0

source "${INSTALL_PATH}/setup.bash"
