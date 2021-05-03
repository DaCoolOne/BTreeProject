

import sys

if __name__ == "__main__":
    test_data = {}
    tree_name = ""
    with open(sys.argv[1]) as f:
        for line in f.readlines():
            l = line.strip()
            if l.startswith("Test "):
                tree_name = l[5:]
            elif l.count(':') == 1:
                test_name, result = l.split(':')
                if test_name not in test_data:
                    test_data[test_name] = []
                test_data[test_name].append((tree_name + ": ").ljust(20) + result.strip())

    print()
    for data in test_data:
        print("Comparison:",data)
        for line in test_data[data]:
            print("   ",line)
        print()

