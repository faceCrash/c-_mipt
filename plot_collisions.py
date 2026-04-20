import csv
import matplotlib.pyplot as plt

def plot_collisions(csv_file, output_image):
    data = {}
    headers = []
    
    try:
        with open(csv_file, mode='r') as f:
            reader = csv.reader(f)
            headers = next(reader)
            
            for h in headers:
                data[h] = []
                
            for row in reader:
                if not row: continue
                for i, val in enumerate(row):
                    data[headers[i]].append(int(val))
        
        plt.figure(figsize=(12, 7))
        x = data[headers[0]] # Size
        
        for h in headers[1:]:
            plt.plot(x, data[h], marker='o', linestyle='-', linewidth=2, markersize=4, label=h)
        
        plt.title('Hash Function Collisions Analysis', fontsize=14)
        plt.xlabel('Number of Strings (Dataset Size)', fontsize=12)
        plt.ylabel('Number of Collisions', fontsize=12)
        plt.legend(title='Hash Functions', bbox_to_anchor=(1.05, 1), loc='upper left')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.tight_layout()
        
        plt.savefig(output_image, dpi=300)
        print(f"Successfully generated plot: {output_image}")
        
    except FileNotFoundError:
        print(f"Error: {csv_file} not found. Please run the C++ program first.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    plot_collisions('collision_results.csv', 'collision_plot.png')
