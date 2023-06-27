make
./sched_sim RR 2 ./example2/p*.txt >./example2/out.txt
python3 analyze_output.py  example2/out.txt