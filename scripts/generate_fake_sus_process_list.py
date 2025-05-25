import os
import random
import hashlib
import csv

def get_sha256(filepath):
    sha256 = hashlib.sha256()
    try:
        with open(filepath, "rb") as f:
            for chunk in iter(lambda: f.read(8192), b""):
                sha256.update(chunk)
        return sha256.hexdigest()
    except Exception as e:
        return None

def walk_files(root="/", limit=10000):
    all_files = []
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            full_path = os.path.join(dirpath, filename)
            all_files.append(full_path)
            if len(all_files) >= limit:
                return all_files
    return all_files

def main():
    root_dir = "C:\\"
    all_files = walk_files(root=root_dir, limit=10000)
    random_files = random.sample(all_files, min(10, len(all_files)))

    results = []
    for file_path in random_files:
        hash_val = get_sha256(file_path)
        if hash_val:
            name = os.path.basename(file_path)
            results.append((name, file_path, hash_val))

    with open("output.csv", "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["name", "path", "sha256"])
        writer.writerows(results)


if __name__ == "__main__":
    main()