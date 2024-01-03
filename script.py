import os

def count_lines(file_path):
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
        return sum(1 for line in file)

def count_lines_in_files(root_folder, extensions):
    total_lines = 0

    for foldername, subfolders, filenames in os.walk(root_folder):
        for filename in filenames:
            if any(filename.lower().endswith(ext) for ext in extensions):
                file_path = os.path.join(foldername, filename)
                lines = count_lines(file_path)
                print(f"{file_path}: {lines} lines")
                total_lines += lines

    return total_lines

if __name__ == "__main__":
    project_folder = os.getcwd()  # Assumes the script is located in the "project" folder
    file_extensions = ['.cpp', '.h']
    total_lines = count_lines_in_files(project_folder, file_extensions)
    print(f"\nTotal lines of code in .cpp and .h files: {total_lines}")
