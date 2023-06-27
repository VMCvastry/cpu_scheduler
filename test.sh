make
./sched_sim RR 2 ./example/p1.txt ./example/p2.txt ./example/p3.txt ./example/p4.txt >./example/out.txt
python3 analyze_output.py  example/out.txt