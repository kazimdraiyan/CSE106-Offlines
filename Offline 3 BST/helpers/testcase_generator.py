# Python BST input/output generator
# Generates a sequence of commands (input) and the corresponding expected outputs
# Matches the format shown by the user (messages, tree prints, traversals, etc.).
#
# Usage: change N to the desired number of operations, or call generate_test(N, seed).
# The script prints the Input block then the Expected Output block.
#
# This code runs here and prints a sample generated test (default N=100).
import random
from typing import Optional, List, Tuple

class Node:
    def __init__(self, key: int):
        self.key = key
        self.left: Optional['Node'] = None
        self.right: Optional['Node'] = None

class BST:
    def __init__(self):
        self.root: Optional[Node] = None
        self._size = 0

    def _repr_default(self, node: Optional[Node]) -> str:
        # Follow formatting rules observed in examples:
        # - Leaf: "(k:k)"
        # - Only left: "(k:k LEFT)"
        # - Only right: "(k:k () RIGHT)"
        # - Both: "(k:k LEFT RIGHT)"
        if node is None:
            return "()"
        left = node.left
        right = node.right
        if left is None and right is None:
            return f"({node.key}:{node.key})"
        if left is not None and right is None:
            return f"({node.key}:{node.key} {self._repr_default(left)})"
        if left is None and right is not None:
            return f"({node.key}:{node.key} () {self._repr_default(right)})"
        # both present
        return f"({node.key}:{node.key} {self._repr_default(left)} {self._repr_default(right)})"

    def repr_default(self) -> str:
        if self.root is None:
            return "Empty"
        return self._repr_default(self.root)

    def _insert_rec(self, node: Optional[Node], key: int) -> Tuple[Optional[Node], bool]:
        if node is None:
            return Node(key), True
        if key == node.key:
            return node, False
        if key < node.key:
            new_left, inserted = self._insert_rec(node.left, key)
            node.left = new_left
            return node, inserted
        else:
            new_right, inserted = self._insert_rec(node.right, key)
            node.right = new_right
            return node, inserted

    def insert(self, key: int) -> Tuple[bool, str]:
        inserted = False
        if self.root is None:
            self.root = Node(key)
            self._size = 1
            inserted = True
        else:
            self.root, inserted = self._insert_rec(self.root, key)
            if inserted:
                self._size += 1
        if inserted:
            return True, f"Key {key} inserted into BST, BST (Default): {self._repr_default(self.root)}"
        else:
            return False, f"Insertion failed! Key {key} already exists in BST, BST (Default): {self._repr_default(self.root)}"

    def find(self, key: int) -> bool:
        cur = self.root
        while cur is not None:
            if key == cur.key:
                return True
            if key < cur.key:
                cur = cur.left
            else:
                cur = cur.right
        return False

    def _find_min_node(self, node: Node) -> Node:
        cur = node
        while cur.left is not None:
            cur = cur.left
        return cur

    def find_min(self) -> Optional[int]:
        if self.root is None:
            return None
        return self._find_min_node(self.root).key

    def find_max(self) -> Optional[int]:
        if self.root is None:
            return None
        cur = self.root
        while cur.right is not None:
            cur = cur.right
        return cur.key

    def _delete_rec(self, node: Optional[Node], key: int) -> Tuple[Optional[Node], bool]:
        if node is None:
            return None, False
        if key < node.key:
            node.left, removed = self._delete_rec(node.left, key)
            return node, removed
        elif key > node.key:
            node.right, removed = self._delete_rec(node.right, key)
            return node, removed
        else:
            # found node to delete
            if node.left is None and node.right is None:
                return None, True
            if node.left is None:
                return node.right, True
            if node.right is None:
                return node.left, True
            # two children: replace with inorder successor (min in right subtree)
            succ = self._find_min_node(node.right)
            node.key = succ.key
            node.right, _ = self._delete_rec(node.right, succ.key)
            return node, True

    def delete(self, key: int) -> Tuple[bool, str]:
        self.root, removed = self._delete_rec(self.root, key)
        if removed:
            self._size -= 1
            if self.root is None:
                return True, f"Key {key} removed from BST, BST (Default): Empty"
            else:
                return True, f"Key {key} removed from BST, BST (Default): {self._repr_default(self.root)}"
        else:
            if self.root is None:
                return False, f"Removal failed! Key {key} not found in BST, BST (Default): Empty"
            else:
                return False, f"Removal failed! Key {key} not found in BST, BST (Default): {self._repr_default(self.root)}"

    def inorder_list(self) -> List[int]:
        res = []
        def dfs(n: Optional[Node]):
            if n is None: return
            dfs(n.left)
            res.append(n.key)
            dfs(n.right)
        dfs(self.root)
        return res

    def preorder_list(self) -> List[int]:
        res = []
        def dfs(n: Optional[Node]):
            if n is None: return
            res.append(n.key)
            dfs(n.left)
            dfs(n.right)
        dfs(self.root)
        return res

    def postorder_list(self) -> List[int]:
        res = []
        def dfs(n: Optional[Node]):
            if n is None: return
            dfs(n.left)
            dfs(n.right)
            res.append(n.key)
        dfs(self.root)
        return res

    def size(self) -> int:
        return self._size

# formatting helpers for traversal output
def fmt_nodes_inline(keys: List[int]) -> str:
    return " ".join(f"({k}:{k})" for k in keys)

# Generator for command sequences and expected outputs
def generate_test(n_ops: int=100, seed: Optional[int]=None) -> Tuple[str, str]:
    if seed is not None:
        random.seed(seed)
    else:
        random.seed()

    b = BST()
    ops = []
    out_lines = []

    # available key range to pick from
    KEY_MIN, KEY_MAX = 1, 200

    # helper to emit command and simulate
    def emit(cmd: str):
        ops.append(cmd)

    # Start with a few warmup commands sometimes
    if random.random() < 0.5:
        k = random.randint(KEY_MIN, KEY_MAX)
        emit(f"F {k}")
        out_lines.append(f"Key {k} not found in BST.")

    if random.random() < 0.4:
        emit("E")
        out_lines.append("Empty" if b.root is None else f"BST (Default): {b.repr_default()}")

    # Build commands until n_ops reached
    while len(ops) < n_ops:
        choice = random.random()
        if choice < 0.30:
            # insert
            k = random.randint(KEY_MIN, KEY_MAX)
            emit(f"I {k}")
            ok, msg = b.insert(k)
            out_lines.append(msg)
        elif choice < 0.45:
            # delete
            k = random.randint(KEY_MIN, KEY_MAX)
            emit(f"D {k}")
            ok, msg = b.delete(k)
            out_lines.append(msg)
        elif choice < 0.55:
            # find
            k = random.randint(KEY_MIN, KEY_MAX)
            emit(f"F {k}")
            found = b.find(k)
            out_lines.append(f"Key {k} {'found' if found else 'not found'} in BST.")
        elif choice < 0.65:
            # Min/Max
            if random.random() < 0.5:
                emit("M Min")
                minv = b.find_min()
                out_lines.append(f"Minimum value: {minv}" if minv is not None else "Empty")
            else:
                emit("M Max")
                maxv = b.find_max()
                out_lines.append(f"Maximum value: {maxv}" if maxv is not None else "Empty")
        elif choice < 0.75:
            # Traversal
            t = random.choice(["In", "Pre", "Post"])
            emit(f"T {t}")
            if t == "In":
                vals = b.inorder_list()
                out_lines.append("BST (In-order): " + fmt_nodes_inline(vals) if vals else "Empty")
            elif t == "Pre":
                vals = b.preorder_list()
                out_lines.append("BST (Pre-order): " + fmt_nodes_inline(vals) if vals else "Empty")
            else:
                vals = b.postorder_list()
                out_lines.append("BST (Post-order): " + fmt_nodes_inline(vals) if vals else "Empty")
        elif choice < 0.85:
            # Size command S
            emit("S")
            out_lines.append(f"Size: {b.size()}")
        else:
            # E command (print default or Empty)
            emit("E")
            out_lines.append("Empty" if b.root is None else f"Not empty")

    # build input and output blocks as multi-line strings
    input_block = "\n".join(ops)
    output_block = "\n".join(out_lines)
    return input_block, output_block

# If run as script: produce a sample test with N=100
N = 800
INPUT, OUTPUT = generate_test(N)

with open("test_case1.txt", "w", encoding="utf-8") as f:
    f.write(INPUT)

with open("test_case_output1.txt", "w", encoding="utf-8") as f:
    f.write(OUTPUT)

print("Test case saved:")
print(" - test_case1.txt")
print(" - test_case_output1.txt")