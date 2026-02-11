#!/usr/bin/env python3
"""
Random test-case generator for the "Assignment on Graphs" problem.
Produces:
 - input.txt  : contains n then list of played matches (one per line as [a, b])
 - output.txt : expected output (number of groups, each group's countries, and missing matches)

Rules respected:
 - countries labeled 0..n-1
 - partition into groups, each group size >= 2
 - matches only inside a group
 - within a group every unordered pair is expected to play exactly once (some may be missing)
 - every country must have played at least one match
"""

import random
import argparse
import itertools
from collections import defaultdict, deque

def partition_into_groups(n, rng):
    """Randomly partition n labeled items into groups of size >=2."""
    labels = list(range(n))
    rng.shuffle(labels)
    groups = []
    idx = 0
    remaining = n
    while remaining > 0:
        if remaining <= 3:
            size = remaining  # 2 or 3
        else:
            # choose size so we don't leave a leftover 1
            max_size = remaining - 2  # leave at least 2 for the rest
            size = rng.randint(2, max_size)
        group = labels[idx: idx + size]
        groups.append(sorted(group))
        idx += size
        remaining -= size
    return groups

def generate_played_matches(groups, p_played, rng):
    """
    For each group, randomly decide which unordered pairs have already been played,
    enforcing that every country has degree >= 1 (i.e., played at least one match).
    Returns a set of directed played matches (winner, loser).
    """
    played_pairs = set()  # unordered pairs as frozenset({u,v})
    directed_matches = []  # list of tuples (winner, loser)

    for group in groups:
        pairs = list(itertools.combinations(group, 2))
        # initial random selection
        for (u, v) in pairs:
            if rng.random() < p_played:
                played_pairs.add(frozenset((u, v)))

        # ensure each node has at least one played match
        deg = {node: 0 for node in group}
        for pair in played_pairs:
            a, b = tuple(pair)
            if a in deg:
                deg[a] += 1
            if b in deg:
                deg[b] += 1

        # For nodes with deg==0, pick a partner and mark that pair played
        for node in group:
            if deg[node] == 0:
                # pick partner from group (not itself)
                partners = [x for x in group if x != node]
                partner = rng.choice(partners)
                played_pairs.add(frozenset((node, partner)))
                deg[node] += 1
                deg[partner] += 1

    # Turn every played unordered pair into a directed result (random winner)
    for pair in played_pairs:
        a, b = tuple(pair)
        if rng.random() < 0.5:
            directed_matches.append((a, b))
        else:
            directed_matches.append((b, a))

    # Sort directed matches (optional) to get deterministic order under a seed
    directed_matches.sort()
    return directed_matches, played_pairs

def reconstruct_groups_and_missing(n, directed_matches, played_pairs):
    """
    Build undirected graph from played_pairs and find connected components (groups).
    For each group, list missing pairs (expected but not played).
    """
    # adjacency undirected
    adj = {i: set() for i in range(n)}
    for pair in played_pairs:
        a, b = tuple(pair)
        adj[a].add(b)
        adj[b].add(a)

    # find connected components (BFS)
    visited = [False] * n
    groups = []
    for i in range(n):
        if not visited[i]:
            # BFS/DFS component
            q = deque([i])
            comp = []
            visited[i] = True
            while q:
                u = q.popleft()
                comp.append(u)
                for v in adj[u]:
                    if not visited[v]:
                        visited[v] = True
                        q.append(v)
            groups.append(sorted(comp))

    # sort groups by smallest label to produce stable ordering
    groups.sort(key=lambda g: g[0])

    # find missing pairs for each group
    missing_for_group = []
    for g in groups:
        missing = []
        for u, v in itertools.combinations(sorted(g), 2):
            if frozenset((u, v)) not in played_pairs:
                missing.append((u, v))
        # sort lexicographically (already in lex order by construction)
        missing_for_group.append(missing)

    return groups, missing_for_group

def format_output(groups, missing_for_group):
    """Return formatted expected output string."""
    lines = []
    lines.append(str(len(groups)))
    lines.append("")  # blank line like sample
    for i, g in enumerate(groups, start=1):
        g_str = ", ".join(str(x) for x in g)
        # missing pairs
        missing = missing_for_group[i-1]
        if not missing:
            missing_str = "none"
        else:
            missing_str = ", ".join(f"[{u},{v}]" for (u, v) in missing)
        lines.append(f"Group {i}: {{{g_str}}} | {missing_str}")
    return "\n".join(lines)

def write_input_file(n, directed_matches, filename="input.txt"):
    with open(filename, "w") as f:
        f.write(f"{n}\n\n")
        for winner, loser in directed_matches:
            f.write(f"[{winner}, {loser}]\n")

def write_output_file(output_str, filename="output.txt"):
    with open(filename, "w") as f:
        f.write(output_str + "\n")

def main():
    parser = argparse.ArgumentParser(description="Generate random testcase for the tournament groups problem.")
    parser.add_argument("--n", type=int, required=True, help="number of countries (>=2)")
    parser.add_argument("--seed", type=int, default=None, help="random seed (optional)")
    parser.add_argument("--p", type=float, default=0.7, help="probability a pair is already played (0..1)")
    parser.add_argument("--input", type=str, default="input.txt", help="input filename to write")
    parser.add_argument("--output", type=str, default="output.txt", help="output filename to write (expected output)")
    args = parser.parse_args()

    n = args.n
    if n < 2:
        raise SystemExit("n must be at least 2.")

    rng = random.Random(args.seed)

    groups = partition_into_groups(n, rng)
    directed_matches, played_pairs = generate_played_matches(groups, args.p, rng)
    groups_recon, missing_for_group = reconstruct_groups_and_missing(n, directed_matches, played_pairs)
    output_str = format_output(groups_recon, missing_for_group)

    write_input_file(n, directed_matches, filename=args.input)
    write_output_file(output_str, filename=args.output)

    print(f"Generated {args.input} and {args.output} for n = {n}.")
    if args.seed is not None:
        print(f"Seed = {args.seed} (reproducible).")
    print("Groups (reconstructed) and missing matches preview:")
    print(output_str)

if __name__ == "__main__":
    main()