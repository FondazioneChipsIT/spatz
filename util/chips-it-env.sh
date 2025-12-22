# Author: Riccardo Giunti, Fondazione Chips-IT

echo "Export Spatz toolchains for Chips-IT"
export LLVM_INSTALL_DIR=/opt/riscv/spatz-14-llvm
export GCC_INSTALL_DIR=/opt/riscv/spatz-gcc-7.1.1

QUESTA_VERSION="2025.3"
echo "Load Questa $QUESTA_VERSION environment for Chips-IT"
module load questa/$QUESTA_VERSION
export VSIM_HOME=/tools/siemens/questa_$QUESTA_VERSION/questasim

export BENDER_INSTALL_DIR=/tools/utils/bender_0.29.1
export VERILATOR_INSTALL_DIR=/tools/verilator/5.041

export PYTHON=python3
if [[ ! -x ".venv/bin/python" ]]; then
  echo "Creating venv in .venv using ${PYTHON}..."
  "${PYTHON}" -m venv ".venv" || { echo "ERROR: venv creation failed"; return 1; }
  echo "Installing Python deps from requirements.txt into .venv"
  .venv/bin/python -m pip install -r requirements.txt || { echo "ERROR: pip install failed"; return 1; }
  . .venv/bin/activate
fi
