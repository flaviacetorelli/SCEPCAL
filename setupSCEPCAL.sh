echo "Set up the env for SCEPCal Simulation"
echo $PWD
source $PWD/init_lcg.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/install/lib64
export PYTHONPATH=$PYTHONPATH:$PWD/install/python
