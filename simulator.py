import subprocess
import os

# --- CONFIGURATION ---
SNAKE_SOLVER_EXE = "snake.exe"        # Ensure this matches your compiled C++ filename
DECODED_FILENAME = "decoded_graphs.txt"
RESULTS_FILENAME = "results.txt"

def main():
    print("--- Graph Snake Solver: Batch Mode ---")

    # 1. Check if the decoded graphs file exists
    if not os.path.exists(DECODED_FILENAME):
        print(f"Error: {DECODED_FILENAME} not found.")
        print("Please ensure your adjacency lists are in that file.")
        return

    # 2. Run C++ Snake Solver
    print(f"Reading graphs from {DECODED_FILENAME}...")
    print(f"Processing via {SNAKE_SOLVER_EXE}...")
    
    try:
        # We open the input file and the output results file
        with open(DECODED_FILENAME, "r") as infile, open(RESULTS_FILENAME, "w") as outfile:
            # We pipe the entire file into the C++ program's stdin
            # The C++ code's while(getline) loop will handle the rest
            subprocess.run([SNAKE_SOLVER_EXE], stdin=infile, stdout=outfile, check=True)
            
    except FileNotFoundError:
        print(f"Error: {SNAKE_SOLVER_EXE} not found. Please compile your C++ code first.")
        return
    except subprocess.CalledProcessError as e:
        print(f"Error: The solver encountered an issue. {e}")
        return

    # 3. Final Output
    print(f"\nDone! Batch processing complete.")
    print(f"Results saved to: {RESULTS_FILENAME}")
    print("-" * 40)
    
    # Optional: Preview the results in the console
    with open(RESULTS_FILENAME, "r") as f:
        print(f.read())

if __name__ == "__main__":
    main()