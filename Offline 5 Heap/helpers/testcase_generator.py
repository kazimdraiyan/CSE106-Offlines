# Python script to generate random heap testcases and their expected outputs.
# It writes two files: input.txt and output.txt in the current working directory.
# Usage (from command-line): python gen_heap_tests.py --n 50 --t 100 --seed 42
# Or call the generate function from within Python.
#
# The generator uses the same command semantics as in your sample:
# 1 x      : insert x
# 2        : extractMin (prints "Extracted Min: <v>" and then heap or "Empty heap")
# 3        : findMin (prints "Min: <v>")
# 8        : print heap (prints "Heap: ...")
# 9        : validate heap (prints "Min Heap property is preserved." / "not preserved")
# 10 k a1 a2 ... ak : heapify from array of length k (prints "Heap built from array." then heap)
# 11       : heapSort (prints "Sorted: ...")
# 12 x     : replaceMin(x) (prints "Replaced Min: <old> with <x>" then heap)
#
# The generator ensures operations are valid (e.g., won't extract from empty heap)
# and respects max capacity `n` for inserts and heapify.
#
# We'll also run an example generation at the end (n=50, t=80) and show the first few lines.
import argparse
import random
import sys
from typing import List

def sift_up(arr: List[int], i: int):
    while i > 0:
        p = (i - 1) // 2
        if arr[i] < arr[p]:
            arr[i], arr[p] = arr[p], arr[i]
            i = p
        else:
            break

def sift_down(arr: List[int], i: int, size: int):
    while True:
        l = 2 * i + 1
        r = 2 * i + 2
        smallest = i
        if l < size and arr[l] < arr[smallest]:
            smallest = l
        if r < size and arr[r] < arr[smallest]:
            smallest = r
        if smallest != i:
            arr[i], arr[smallest] = arr[smallest], arr[i]
            i = smallest
        else:
            break

def build_heap_from_array(arr: List[int]):
    size = len(arr)
    # bottom-up heapify
    for i in range(size // 2 - 1, -1, -1):
        sift_down(arr, i, size)

def is_min_heap(arr: List[int]) -> bool:
    n = len(arr)
    for i in range(n):
        l = 2 * i + 1
        r = 2 * i + 2
        if l < n and arr[i] > arr[l]:
            return False
        if r < n and arr[i] > arr[r]:
            return False
    return True

class HeapSimulator:
    def __init__(self, capacity: int):
        self.capacity = capacity
        self.arr: List[int] = []

    def insert(self, x: int):
        if len(self.arr) >= self.capacity:
            raise RuntimeError("Capacity exceeded on insert")
        self.arr.append(x)
        sift_up(self.arr, len(self.arr) - 1)

    def find_min(self) -> int:
        if not self.arr:
            raise RuntimeError("Empty heap")
        return self.arr[0]

    def extract_min(self) -> int:
        if not self.arr:
            raise RuntimeError("Empty heap")
        mn = self.arr[0]
        # replace root with last
        last = self.arr.pop()
        if self.arr:
            self.arr[0] = last
            sift_down(self.arr, 0, len(self.arr))
        return mn

    def heapify_from_array(self, arr: List[int]):
        if len(arr) > self.capacity:
            raise RuntimeError("Capacity exceeded on heapify")
        self.arr = arr[:]
        build_heap_from_array(self.arr)

    def heap_sort(self) -> List[int]:
        # Return sorted ascending, leave self.arr unchanged.
        copy = self.arr[:]
        build_heap_from_array(copy)
        res = []
        while copy:
            res.append(copy[0])
            last = copy.pop()
            if copy:
                copy[0] = last
                sift_down(copy, 0, len(copy))
        return res

    def replace_min(self, x: int) -> int:
        if not self.arr:
            raise RuntimeError("Empty heap")
        old = self.arr[0]
        self.arr[0] = x
        sift_down(self.arr, 0, len(self.arr))
        return old

    def get_heap_list(self) -> List[int]:
        return self.arr[:]

def generate_testcase(n: int, t: int, seed: int = None, out_input="gen_input.txt", out_output="gen_output.txt"):
    if seed is not None:
        random.seed(seed)

    sim = HeapSimulator(capacity=n)
    input_lines: List[str] = []
    output_lines: List[str] = []

    # Operation set
    ops = ["insert", "extract", "find", "print", "validate", "heapify", "heapsort", "replace"]
    # We'll bias probabilities sensibly:
    weights = [0.25, 0.10, 0.08, 0.12, 0.05, 0.10, 0.10, 0.20]  # sum 1.0 approximate

    for step in range(t):
        # pick op but ensure it's valid
        # if heap empty, avoid extract/find/replace/print? print/validate/heapify/heapsort are allowed
        valid_ops = ops[:]
        valid_weights = weights[:]

        if not sim.arr:
            # remove extract, find, replace
            for bad in ["extract", "find", "replace"]:
                if bad in valid_ops:
                    idx = valid_ops.index(bad)
                    valid_ops.pop(idx)
                    valid_weights.pop(idx)
        else:
            # if heap is full, avoid insert and heapify with large arrays
            if len(sim.arr) >= n:
                if "insert" in valid_ops:
                    idx = valid_ops.index("insert")
                    valid_ops.pop(idx)
                    valid_weights.pop(idx)

        # normalize weights
        total = sum(valid_weights)
        normalized = [w / total for w in valid_weights]

        op = random.choices(valid_ops, weights=normalized, k=1)[0]

        if op == "insert":
            # choose value between -100 and 200 to include variety
            x = random.randint(-100, 200)
            input_lines.append(f"1 {x}")
            try:
                sim.insert(x)
                output_lines.append(f"Inserted {x} into the heap.")
            except RuntimeError as e:
                # shouldn't happen due to checks but handle gracefully
                output_lines.append(str(e))

        elif op == "extract":
            input_lines.append("2")
            input_lines.append("8")
            val = sim.extract_min()
            output_lines.append(f"Extracted Min: {val}")
            if sim.arr:
                output_lines.append("Heap: " + " ".join(str(v) for v in sim.get_heap_list()))
            else:
                output_lines.append("Empty heap")

        elif op == "find":
            input_lines.append("3")
            val = sim.find_min()
            output_lines.append(f"Min: {val}")

        elif op == "print":
            input_lines.append("8")
            if sim.arr:
                output_lines.append("Heap: " + " ".join(str(v) for v in sim.get_heap_list()))
            else:
                output_lines.append("Empty heap")

        elif op == "validate":
            input_lines.append("9")
            if is_min_heap(sim.get_heap_list()):
                output_lines.append("Min Heap property is preserved.")
            else:
                output_lines.append("Min Heap property is not preserved.")

        elif op == "heapify":
            # produce an array of length k between 1 and n (but not exceeding capacity)
            if n == 0:
                # degenerate, produce no-op print
                input_lines.append("8")
                output_lines.append("Empty heap")
                continue
            k = random.randint(1, n)
            arr = [random.randint(-200, 300) for _ in range(k)]
            input_lines.append("10 " + " ".join([str(k)] + [str(x) for x in arr]))
            input_lines.append("8")
            sim.heapify_from_array(arr)
            output_lines.append("Heap built from array.")
            output_lines.append("Heap: " + " ".join(str(v) for v in sim.get_heap_list()))

        elif op == "heapsort":
            input_lines.append("11")
            sorted_list = sim.heap_sort()
            if sorted_list:
                output_lines.append("Sorted: " + " ".join(str(v) for v in sorted_list))
            else:
                output_lines.append("Sorted:")

        elif op == "replace":
            # choose x and do replaceMin
            x = random.randint(-500, 500)
            input_lines.append(f"12 {x}")
            input_lines.append("8")
            old = sim.replace_min(x)
            output_lines.append(f"Replaced Min: {old} with {x}")
            output_lines.append("Heap: " + " ".join(str(v) for v in sim.get_heap_list()))

    # Write files
    with open(out_input, "w") as f_in:
        for line in input_lines:
            f_in.write(line + "\n")

    with open(out_output, "w") as f_out:
        for line in output_lines:
            f_out.write(line + "\n")

    return out_input, out_output

# If run as script, parse args and generate
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate random heap testcases and expected output.")
    parser.add_argument("--n", type=int, required=False, default=50, help="max number of elements in heap (capacity)")
    parser.add_argument("--t", type=int, required=False, default=80, help="number of operations to generate")
    parser.add_argument("--seed", type=int, required=False, default=42, help="random seed (optional)")
    parser.add_argument("--input", type=str, default="input.txt", help="input filename to write")
    parser.add_argument("--output", type=str, default="output.txt", help="expected output filename to write")

    args = parser.parse_args()
    inp, outp = generate_testcase(args.n, args.t, seed=args.seed, out_input=args.input, out_output=args.output)
    print(f"Generated {inp} and {outp} (n={args.n}, t={args.t}, seed={args.seed})")
