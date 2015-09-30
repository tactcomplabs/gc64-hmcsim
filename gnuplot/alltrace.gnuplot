#-- alltrace.gnuplot

set title "HMC-SIM" 
set xlabel "Clock Tick" 
set ylabel "Count" 
#set ytics "10"
set term png
set output "1024.png"

plot "bank_conflict.out" title "Bank Conflicts" with linespoints,\
     "xbar_latency.out" title "XBAR Latency" with linespoints,\
     "xbar_rqst_stall.out" title "XBAR Request Stall" with linespoints,\
     "rd64.out" title "RD64 CMD" with linespoints,\
     "wr64.out" title "WR64 CMD" with linespoints
