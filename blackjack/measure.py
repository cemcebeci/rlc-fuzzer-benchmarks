from subprocess import run
from statistics import mean
import time
import matplotlib.pyplot as plt

REPEAT_COUNT = 4
fuzzers = ['rlc_blackjack', 'rlc_blackjack_no_fsm', 'rlc_blackjack_no_precons', 'rlc_blackjack_no_improvements', 'open_spiel_whitebox_blackjack', 'open_spiel_blackbox_blackjack']
bug_depths = range(1, 30, 5)

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
            run(f'build/{fuzzer} -max_total_time=120 >/dev/null 2>&1', shell=True)
            end = time.time()
            time_measurements[fuzzer][bug_depth].append(end - start)

    processed_depths = bug_depths[:j]
    print(processed_depths)

    average_measurements = {fuzzer:[mean(time_measurements[fuzzer][bug_depth]) for bug_depth in processed_depths] for fuzzer in fuzzers}
    print(average_measurements)

    fig, ax = plt.subplots() 
    ax.plot(processed_depths, average_measurements['rlc_blackjack'], 'g-', label='rlc-full')
    ax.plot(processed_depths, average_measurements['rlc_blackjack_no_fsm'], 'b-', label='rlc-no-fsm')
    ax.plot(processed_depths, average_measurements['rlc_blackjack_no_precons'], 'c-', label='rlc-no-precons')
    ax.plot(processed_depths, average_measurements['rlc_blackjack_no_improvements'], 'y-', label='rlc-no-improvements')
    ax.plot(processed_depths, average_measurements['open_spiel_blackbox_blackjack'], 'r-', label='os-blackbox')
    ax.plot(processed_depths, average_measurements['open_spiel_whitebox_blackjack'], 'm-', label='os-whitebox')

    plt.xlabel('Bug Depth')
    plt.ylabel('Average Fuzzing Time (s)')

    ax.legend()
    plt.savefig(f'plot_{j}.png')
    plt.clf()

    for fuzzer in fuzzers:
        print("---------------------" + fuzzer.capitalize() + "-------------------")
        for bug_depth in processed_depths:
            print("\t".join([str(measurement) for measurement in time_measurements[fuzzer][bug_depth]]))