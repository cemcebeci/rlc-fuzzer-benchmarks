from subprocess import run
from statistics import mean
import time
import matplotlib.pyplot as plt

REPEAT_COUNT = 3
fuzzers = ['fuzzer_blackjack', 'fuzzer_blackjack_no_fsm']#, 'fuzzer_blackjack_no_precons', 'fuzzer_blackjack_no_improvements']
bug_depths = range(1, 30, 2)

time_measurements = {fuzzer: {} for fuzzer in fuzzers}


j = 0
for bug_depth in bug_depths:
    j = j + 1
    run(['make', 'clean'])
    run(['make', f'BUG_DEPTH={bug_depth}'])
    for fuzzer in fuzzers:
        time_measurements[fuzzer][bug_depth] = []
        print(f"{bug_depth} - {fuzzer}")
        for i in range(REPEAT_COUNT):
            start = time.time()
            run(f'build/{fuzzer} -rss_limit_mb=4096 -detect_leaks=0 -max_total_time=240 >/dev/null 2>&1', shell=True)
            end = time.time()
            time_measurements[fuzzer][bug_depth].append(end - start)

    processed_depths = bug_depths[:j]
    print(processed_depths)

    average_measurements = {fuzzer:[mean(time_measurements[fuzzer][bug_depth]) for bug_depth in processed_depths] for fuzzer in fuzzers}
    print(average_measurements)

    fig, ax = plt.subplots() 
    ax.plot(processed_depths, average_measurements['fuzzer_blackjack'], 'g-', label='full')
    ax.plot(processed_depths, average_measurements['fuzzer_blackjack_no_fsm'], 'b-', label='no-fsm')

    ax.legend()
    plt.savefig(f'plot_{j}.png')
    plt.clf()

    for fuzzer in fuzzers:
        print("---------------------" + fuzzer.capitalize() + "-------------------")
        for bug_depth in processed_depths:
            print("\t".join([str(measurement) for measurement in time_measurements[fuzzer][bug_depth]]))