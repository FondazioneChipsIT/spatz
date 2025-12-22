# Author: Riccardo Giunti, Fondazione Chips-IT

echo "Export Spatz toolchains for Chips-IT"
export LLVM_INSTALL_DIR=/opt/riscv/spatz-15-llvm
export GCC_INSTALL_DIR=/opt/riscv/spatz-gcc-7.1.1
export VERDI_HOME=tools/synopsys/verdi/W-2024.09-SP1

QUESTA_VERSION="2025.3"
echo "Load Questa $QUESTA_VERSION environment for Chips-IT"
module load questa/$QUESTA_VERSION

VCS_VERSION="2024.09"
echo "Load VCS $VCS_VERSION environment for Chips-IT"
module load vcs/$VCS_VERSION

BENDER_VERSION="0.31.0"
echo "Load Bender $BENDER_VERSION environment for Chips-IT"
module load bender/$BENDER_VERSION
export BENDER_INSTALL_DIR=/tools/utils/bender_$BENDER_VERSION


export PYTHON=python3.8
if [[ ! -x ".venv/bin/python" ]]; then
  echo "Creating venv in .venv using ${PYTHON}..."
  "${PYTHON}" -m venv ".venv" || { echo "ERROR: venv creation failed"; return 1; }
fi
  echo "Installing Python deps from requirements.txt into .venv"
  .venv/bin/python -m pip install -r requirements.txt || { echo "ERROR: pip install failed"; return 1; }
  . .venv/bin/activate
