#!/usr/bin/env bash
# Build the walker_motion variant of p73_cc (63D motion-tracking obs) into a
# separate install tree so it does NOT overwrite the default walker (47D) install/.
#
# Usage:
#   1. cd src/p73_cc && git checkout walker_motion && cd ../..
#   2. ./build_motion.sh            # builds ALL packages into install_motion/
#   3. cd src/p73_cc && git checkout main && cd ../..   # restore src
#
# At runtime, source install_motion/setup.bash for walker_motion eval,
# and install/setup.bash for walker eval. The two install trees are frozen
# binaries — the src branch afterwards does not matter.
set -euo pipefail

WS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$WS_DIR"

CUR_BRANCH="$(git -C src/p73_cc branch --show-current)"
if [ "$CUR_BRANCH" != "walker_motion" ]; then
  echo "WARNING: src/p73_cc is on branch '$CUR_BRANCH', not 'walker_motion'." >&2
  echo "         install_motion/ should be built from the walker_motion branch." >&2
  read -r -p "Continue anyway? [y/N] " ans
  [ "$ans" = "y" ] || [ "$ans" = "Y" ] || { echo "Aborted."; exit 1; }
fi

# Separate --build-base is REQUIRED: CMake packages bake CMAKE_INSTALL_PREFIX into
# the build tree at configure time. Reusing the default build/ (configured for
# install/) would install binaries into install/ (or skip them), leaving
# install_motion/ without lib/. A dedicated build_motion/ configures cleanly.
#
# p73_lib finds ONNX Runtime via find_library(onnxruntime HINTS /usr/local/lib),
# but that symlink is broken on this host. p73_cc bundles a valid copy in
# thirdparty/onnxruntime — add it to CMAKE_PREFIX_PATH so find_library locates it
# (this is how install/ was originally built).
export CMAKE_PREFIX_PATH="$WS_DIR/src/p73_cc/thirdparty/onnxruntime:${CMAKE_PREFIX_PATH:-}"

# Force system python3: rosidl message generation (p73_msgs) needs the `em` (empy)
# module, which the conda python lacks. install/ was also built with /usr/bin/python3.
colcon build --build-base build_motion --install-base install_motion \
  --cmake-args -DPython3_EXECUTABLE=/usr/bin/python3 "$@"
echo "Done. Built into $WS_DIR/install_motion"
