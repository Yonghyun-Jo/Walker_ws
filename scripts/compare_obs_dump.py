#!/usr/bin/env python3
"""Compare IsaacLab vs MuJoCo obs dumps for Walker sim-to-sim debugging.

Usage:
    python compare_obs_dump.py \
        --isaac /tmp/walker_isaac_obs.jsonl \
        --mujoco /tmp/walker_mujoco_obs.jsonl \
        --max_steps 10
"""
import argparse
import json
import sys

import numpy as np

TERM_NAMES = ["ang_vel", "gravity", "cmd", "gait_sin", "gait_cos",
              "joint_pos", "joint_vel", "last_action"]
TERM_DIMS = [3, 3, 3, 1, 1, 12, 12, 12]


def load_jsonl(path):
    records = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                records.append(json.loads(line))
            except json.JSONDecodeError:
                continue
    return records


def fmt(v, w=10):
    if isinstance(v, (list, np.ndarray)):
        return " ".join(f"{x:>{w}.6f}" for x in v)
    return f"{v:>{w}.6f}"


def compare_term(name, isaac_vals, mujoco_vals):
    """Compare a single observation term. Returns max abs diff."""
    iv = np.array(isaac_vals).flatten()
    mv = np.array(mujoco_vals).flatten()
    diff = iv - mv
    max_diff = np.max(np.abs(diff))

    # Color: green if close, yellow if moderate, red if large
    if max_diff < 0.01:
        status = "OK"
    elif max_diff < 0.1:
        status = "WARN"
    else:
        status = "MISMATCH"

    return max_diff, diff, iv, mv, status


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--isaac", default="/tmp/walker_isaac_obs.jsonl")
    parser.add_argument("--mujoco", default="/tmp/walker_mujoco_obs.jsonl")
    parser.add_argument("--max_steps", type=int, default=10)
    parser.add_argument("--per_dim", action="store_true", help="Show per-dimension diff")
    args = parser.parse_args()

    isaac_data = load_jsonl(args.isaac)
    mujoco_data = load_jsonl(args.mujoco)

    n = min(len(isaac_data), len(mujoco_data), args.max_steps)
    if n == 0:
        print("ERROR: No data to compare.")
        sys.exit(1)

    print(f"Comparing {n} steps  (isaac={len(isaac_data)}, mujoco={len(mujoco_data)})")
    print("=" * 90)

    for step in range(n):
        ir = isaac_data[step]
        mr = mujoco_data[step]

        print(f"\n{'='*90}")
        print(f"STEP {step}")
        print(f"{'='*90}")

        # Compare per-term newest frame
        iframe = ir.get("frame_47", {})
        mframe = mr.get("frame_47", {})

        for name in TERM_NAMES:
            iv = iframe.get(name, [])
            mv = mframe.get(name, [])

            # Handle scalar vs list
            if isinstance(iv, (int, float)):
                iv = [iv]
            if isinstance(mv, (int, float)):
                mv = [mv]

            if len(iv) == 0 or len(mv) == 0:
                print(f"  {name:>14s}: MISSING DATA")
                continue

            max_diff, diff, iv_arr, mv_arr, status = compare_term(name, iv, mv)
            print(f"  {name:>14s}  max_diff={max_diff:.6f}  [{status}]")

            if args.per_dim or status != "OK":
                for d in range(len(iv_arr)):
                    marker = " ***" if abs(diff[d]) > 0.01 else ""
                    print(f"    [{d:2d}]  isaac={iv_arr[d]:>10.6f}  mujoco={mv_arr[d]:>10.6f}  diff={diff[d]:>10.6f}{marker}")

        # Compare actions
        ia = np.array(ir.get("actions", []))
        ma = np.array(mr.get("actions", []))
        if len(ia) > 0 and len(ma) > 0:
            act_diff = ia - ma
            max_act = np.max(np.abs(act_diff))
            status = "OK" if max_act < 0.01 else ("WARN" if max_act < 0.1 else "MISMATCH")
            print(f"  {'actions':>14s}  max_diff={max_act:.6f}  [{status}]")
            if args.per_dim or status != "OK":
                for d in range(len(ia)):
                    marker = " ***" if abs(act_diff[d]) > 0.01 else ""
                    print(f"    [{d:2d}]  isaac={ia[d]:>10.6f}  mujoco={ma[d]:>10.6f}  diff={act_diff[d]:>10.6f}{marker}")

        # Compare full obs (235D) summary
        io = np.array(ir.get("obs_235", []))
        mo = np.array(mr.get("obs_235", []))
        if len(io) == len(mo) and len(io) > 0:
            full_diff = np.abs(io - mo)
            print(f"  {'obs_235 (full)':>14s}  max_diff={np.max(full_diff):.6f}  mean_diff={np.mean(full_diff):.6f}")

    print(f"\n{'='*90}")
    print("DONE")


if __name__ == "__main__":
    main()
