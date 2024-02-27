from subprocess import run
from statistics import mean
import time
import matplotlib.pyplot as plt

REPEAT_COUNT = 2
fuzzers = ['open_spiel_whitebox_crazy_eights']
bug_depths = range(7, 26, 2)

time_measurements = {fuzzer: {} for fuzzer in fuzzers}
execution_count_measurements = {fuzzer: {} for fuzzer in fuzzers}


j = 0
for bug_depth in bug_depths:
    j = j + 1
    run(['make', 'clean'])
    run(['make', f'BUG_DEPTH={bug_depth}'])
    for fuzzer in fuzzers:
        time_measurements[fuzzer][bug_depth] = []
        execution_count_measurements[fuzzer][bug_depth] = []
        print(f"{bug_depth} - {fuzzer}")
        for i in range(REPEAT_COUNT):
            start = time.time()
            sp = run([f'build/{fuzzer}','-max_total_time=150', '-print_final_stats=1'], capture_output=True)
            end = time.time()
            time_measurements[fuzzer][bug_depth].append(end - start)
            execution_count = (int)(sp.stderr.decode().splitlines()[-5].split()[-1])
            execution_count_measurements[fuzzer][bug_depth].append(execution_count)

    processed_depths = bug_depths[:j]
    print(processed_depths)

    average_time_measurements = {fuzzer:[mean(time_measurements[fuzzer][bug_depth]) for bug_depth in processed_depths] for fuzzer in fuzzers}
    print(average_time_measurements)
    average_count_measurements = {fuzzer:[mean(execution_count_measurements[fuzzer][bug_depth]) for bug_depth in processed_depths] for fuzzer in fuzzers}

    fig, ax = plt.subplots() 
    #ax.plot(processed_depths, average_time_measurements['rlc_tic_tac_toe'], 'g-', label='rlc-full')
    #ax.plot(processed_depths, average_time_measurements['rlc_tic_tac_toe_no_fsm'], 'b-', label='rlc-no-fsm')
    # ax.plot(processed_depths, average_time_measurements['rlc_tic_tac_toe_no_precons'], 'c-', label='rlc-no-precons')
    # ax.plot(processed_depths, average_time_measurements['rlc_tic_tac_toe_no_improvements'], 'y-', label='rlc-blackbox')
    # ax.plot(processed_depths, average_time_measurements['open_spiel_blackbox_tic_tac_toe'], 'r-', label='os-blackbox')
    ax.plot(processed_depths, average_time_measurements['open_spiel_whitebox_crazy_eights'], 'm-', label='os-whitebox')

    plt.xlabel('Bug Depth')
    plt.ylabel('Average Fuzzing Time (s)')

    ax.legend()
    plt.savefig(f'total_times_{j}.png')
    plt.clf()

    for fuzzer in fuzzers:
        print("---------------------" + fuzzer.capitalize() + "-------------------")
        for bug_depth in processed_depths:
            print("\t".join([str(measurement) for measurement in time_measurements[fuzzer][bug_depth]]))

    fig, ax = plt.subplots() 
    # ax.plot(processed_depths, average_count_measurements['rlc_tic_tac_toe'], 'g-', label='rlc-full')
    # ax.plot(processed_depths, average_count_measurements['rlc_tic_tac_toe_no_fsm'], 'b-', label='rlc-no-fsm')
    # ax.plot(processed_depths, average_count_measurements['rlc_tic_tac_toe_no_precons'], 'c-', label='rlc-no-precons')
    # ax.plot(processed_depths, average_count_measurements['rlc_tic_tac_toe_no_improvements'], 'y-', label='rlc-blackbox')
    # ax.plot(processed_depths, average_count_measurements['open_spiel_blackbox_tic_tac_toe'], 'r-', label='os-blackbox')
    ax.plot(processed_depths, average_count_measurements['open_spiel_whitebox_crazy_eights'], 'm-', label='os-whitebox')
    plt.yscale("log")
    plt.xlabel('Bug Depth')
    plt.ylabel('Average Number of Executions')

    ax.legend()
    plt.savefig(f'num_executions_{j}.png')
    plt.clf()

    for fuzzer in fuzzers:
        print("---------------------" + fuzzer.capitalize() + "-------------------")
        for bug_depth in processed_depths:
            print("\t".join([str(measurement) for measurement in execution_count_measurements[fuzzer][bug_depth]]))