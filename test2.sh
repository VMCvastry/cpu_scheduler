make
./sched_sim ./example2/p*.txt >./example2/out.txt
python3 analyze_output.py  example2/out.txt