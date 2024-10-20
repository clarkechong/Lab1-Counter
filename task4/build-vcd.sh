rm -r ./obj_dir/
rm -f ./counter.vcd

verilator -Wall --cc --trace top.sv counter.sv bin2bcd.sv --exe top_tb.cpp
make -j -C ./obj_dir/ -f Vtop.mk Vtop
./obj_dir/Vtop