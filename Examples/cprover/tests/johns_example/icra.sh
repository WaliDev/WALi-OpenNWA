#~/Newton/WALi-OpenNWA/_build64/Examples/cprover/NewtonOcaml -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=temp_result.out $1 |& tee temp_output.txt
ABS=$(readlink -f $1)
cd ~/Newton/WALi-OpenNWA/
_build64/Examples/cprover/NewtonOcaml -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=temp_result.out $ABS |& tee temp_output.txt
