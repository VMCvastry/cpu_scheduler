make
./sched_sim PRED_SJF 10 ./example2/p*.txt >./example2/out.txt
python3 analyze_output.py  example2/out.txt