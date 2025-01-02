import numpy as np
import matplotlib.pyplot as plt
import os
import re
from collections import defaultdict

algorithms_data_times = {
    'BranchAndBound': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'BestFirstSearch': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'AStar': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'Bruteforce': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'Random': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'NearestNeighbor': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
}

algorithms_data_costs = {
    'BranchAndBound': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'BestFirstSearch': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'AStar': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'Bruteforce': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'Random': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
    'NearestNeighbor': {'tsp': [], 'identyczne_tsp': [], 'niepelne_asymetryczne': []},
}

def get_file_type(filename):
    if filename.startswith('tsp'):
        return 'tsp'
    elif filename.startswith('identyczne_tsp'):
        return 'identyczne_tsp'
    elif filename.startswith('niepelne_asymetryczne_tsp'):
        return 'niepelne_asymetryczne'
    return None

def extract_number_from_filename(filename): # uzyskanie liczby z nazwy pliku
    match = re.search(r'\d+', filename)
    if match:
        return int(match.group())
    return float('inf')  # jesli nie znaleziono liczby ustawiamy wartosć nieskonczona

colors = {'tsp': 'blue', 'identyczne_tsp': 'orange', 'niepelne_asymetryczne': 'purple'}

def process_times_file(file_path):
    aggregated_times = defaultdict(list)  # grupujemy po pliku i algorytmie 
    with open(file_path, 'r') as file:
        lines = file.readlines()

        for line in lines:
            parts = line.strip().split()
            if len(parts) != 3:
                continue

            filename, algorithm, time_str = parts
            try:
                time = int(time_str)
            except ValueError:
                time = 0

            file_type = get_file_type(filename)
            if file_type and algorithm in algorithms_data_times:
                aggregated_times[(algorithm, file_type)].append((filename, time))

    # srednia i blad
    for (algorithm, file_type), values in aggregated_times.items():
        grouped = defaultdict(list)
        for filename, time in values:
            grouped[filename].append(time)

        for filename, times in grouped.items():
            mean_time = np.mean(times)
            std_time = np.std(times)
            algorithms_data_times[algorithm][file_type].append((filename, mean_time, std_time))

def process_costs_file(file_path):
    aggregated_costs = defaultdict(list)
    with open(file_path, 'r') as file:
        lines = file.readlines()

        for line in lines:
            parts = line.strip().split()
            if len(parts) != 4:
                continue

            filename, algorithm, cost_found, cost_optimal = parts

            try:
                cost_found = float(cost_found)
                cost_optimal = float(cost_optimal)
                error = ((cost_found - cost_optimal) / cost_found) * 100
            except ValueError:
                continue

            file_type = get_file_type(filename)
            if file_type and algorithm in algorithms_data_costs:
                aggregated_costs[(algorithm, file_type)].append((filename, cost_found, error))

    # srednia i blad
    for (algorithm, file_type), values in aggregated_costs.items():
        grouped = defaultdict(list)
        for filename, cost_found, error in values:
            grouped[filename].append((cost_found, error))

        for filename, cost_error_list in grouped.items():
            mean_cost = np.mean([c for c, _ in cost_error_list])
            mean_error = np.mean([e for _, e in cost_error_list])
            std_error = np.std([e for _, e in cost_error_list])
            algorithms_data_costs[algorithm][file_type].append((filename, mean_cost, mean_error, std_error))

folder_path = "cmake-build-debug"
diagrams_folder = "diagrams"
os.makedirs(diagrams_folder, exist_ok=True)

files_to_check = ["results_costs.txt", "results.txt", "results_times.txt"]

# przetwarzanie plikow
for file_name in files_to_check:
    file_path = os.path.join(folder_path, file_name)
    if os.path.exists(file_path):
        if "times" in file_name:
            process_times_file(file_path)
        elif "costs" in file_name:
            process_costs_file(file_path)

# diagramy czasow
for algorithm, data in algorithms_data_times.items():
    plt.figure(figsize=(10, 6))
    has_data = False
    for file_type, values in data.items():
        if values:
            has_data = True
            values.sort(key=lambda x: extract_number_from_filename(x[0]))
            sizes, means, stds = zip(*[
                (extract_number_from_filename(fname), mean, std)
                for fname, mean, std in values
            ])
            plt.errorbar(sizes, means, yerr=stds, fmt='o-', color=colors[file_type], label=file_type)

    if has_data:
        plt.xlabel('liczba wierzcholkow')
        plt.ylabel('czas wykonania [ms]')
        plt.title(f'czas wykonania - algorytm: {algorithm}')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(diagrams_folder, f"{algorithm}_times.png"))
        plt.close()

# diagramy bledow (nie dzialaja)
for algorithm, data in algorithms_data_costs.items():
    plt.figure(figsize=(10, 6))
    has_data = False
    for file_type, values in data.items():
        if values:
            has_data = True
            values.sort(key=lambda x: extract_number_from_filename(x[0]))
            sizes, means, stds = zip(*[
                (extract_number_from_filename(fname), mean, std)
                for fname, mean, std in values
            ])
            plt.errorbar(sizes, means, yerr=stds, fmt='o-', color=colors[file_type], label=file_type)

    if has_data:
        plt.xlabel('koszt znaleziony')
        plt.ylabel('blad [%]')
        plt.title(f'blad - algorytm: {algorithm}')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(diagrams_folder, f"{algorithm}_errors.png"))
        plt.close()
