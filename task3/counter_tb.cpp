#include "./obj_dir/Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "../vbuddy.cpp"

int main(int argc, char** argv, char** env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    Vcounter* top = new Vcounter; // instantiate counter.sv module
    VerilatedVcdC* tfp = new VerilatedVcdC; // "vcd" meaning value change dump. just a file to store waveform data

    Verilated::traceEverOn(true);

    top->trace (tfp, 99);   // "top" to reference the top level entity of the hierarchy
    tfp->open ("counter.vcd");  // "tfp" for trace file pointer

    top->clk = 1;
    top->rst = 0;
    // top->v = 0xAA;

    // init Vbuddy
    if (vbdOpen()!=1){
        return (-1);
    }
    vbdSetMode(1);
    vbdHeader("task 3 shiz");

    for (i=0; i<1000; i++){

        for (clk=0; clk<2; clk++){
            tfp->dump (2*i+clk); // this expression represents simulation time. dump() records all the values at this timestamp
            top->clk = !top->clk; // toggle value of clk between 0/1
            top->eval (); // a bit like a sleep() or wait(). it requests the simulation to evaluate all current inputs
        }

        top->ld = vbdFlag();

        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);

        // vbdPlot(int(top->count), 0, 255);

        vbdCycle(i+1);

        if (Verilated::gotFinish()) exit (0);
    }
    vbdClose();
    tfp->close();
    exit(0);

}