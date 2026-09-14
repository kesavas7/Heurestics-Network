import sys
import random
def solve():
    data = sys.stdin.read().split()
    if not data:
        return
    idx = 0
    N, M, D = int(data[idx]), int(data[idx+1]), int(data[idx+2])
    idx += 3
    V = list(map(int, data[idx:idx+N]))
    idx += N
    facto = []
    for j in range(M):
        K = int(data[idx])
        T = int(data[idx+1])
        idx += 2
        pick = list(map(int, data[idx:idx+K]))
        pick.sort(key=lambda x: V[x], reverse=True)
        idx += K
        facto.append((T, j, K, pick))
    if N > 50000 or M > 5000:
        facto.sort(key=lambda x: x[0])
        ti = 0
        par = []
        used = [False] * N
        for T, fid, K, ax in facto:
            if ti + T >= D:
                break
            ti += T
            rem = D - ti
            p = min(K, rem)
            gg = []
            for axe in ax:
                if not used[axe]:
                    gg.append(axe)
                    used[axe] = True
                    if len(gg) == p:
                        break
            par.append((fid, gg))
        print(len(par))
        for fid, r in par:
            print(fid, len(r), *r)
        return
    factory_score = []
    for T, fid, K, ax in facto:
        if T >= D:
            continue
        p = min(K, D - T)
        value = sum(V[x] for x in ax[:p])
        score = value / T
        factory_score.append((score, T, fid, K, ax))
    factory_score.sort(reverse=True)
    facto = [(T, fid, K, ax) for score, T, fid, K, ax in factory_score]
    facto_by_id = {fid: (T, K, ax) for T, fid, K, ax in facto}
    BEAM_WIDTH = 10
    CANDIDATES = 10
    beam = [(0, 0, [], frozenset())]
    while True:
        new_states = []
        for total_value, ti, selected, used in beam:
            candidates = []
            for T, fid, K, ax in facto:
                if fid in selected or ti + T >= D:
                    continue
                new_time = ti + T
                remaining_days = D - new_time
                p = min(K, remaining_days)
                gain = 0
                count = 0
                new_pickaxes = []
                for x in ax:
                    if x not in used:
                        gain += V[x]
                        new_pickaxes.append(x)
                        count += 1
                        if count == p:
                            break
                if count == 0:
                    continue
                score = gain / T
                candidates.append((score, T, fid, new_time, gain, new_pickaxes))
            candidates.sort(reverse=True)
            for score, T, fid, new_time, gain, new_pickaxes in candidates[:CANDIDATES]:
                new_used = used.union(new_pickaxes)
                new_selected = selected + [fid]
                new_states.append((total_value + gain, new_time, new_selected, new_used))
        if not new_states:
            break
        new_states.sort(key=lambda x: x[0], reverse=True)
        beam = new_states[:BEAM_WIDTH]
    def evaluate_solution(seq):
        curr_t = 0
        val = 0
        used_p = [False] * N
        valid_seq = []
        for fid in seq:
            if fid not in facto_by_id:
                continue
            T, K, ax = facto_by_id[fid]
            if curr_t + T >= D:
                continue
            curr_t += T
            rem = D - curr_t
            p = min(K, rem)
            cnt = 0
            for x in ax:
                if not used_p[x]:
                    val += V[x]
                    used_p[x] = True
                    cnt += 1
                    if cnt == p:
                        break
            valid_seq.append(fid)
        return val, valid_seq
    best_beam = max(beam, key=lambda x: x[0])
    best_seq = best_beam[2]
    best_val, best_seq = evaluate_solution(best_seq)
    MAX_LS_PASSES = 100
    ls_count = 0
    improved = True
    while improved and ls_count < MAX_LS_PASSES:
        improved = False
        ls_count += 1
        for i in range(len(best_seq) - 1):
            best_seq[i], best_seq[i+1] = best_seq[i+1], best_seq[i]
            val, seq = evaluate_solution(best_seq)
            if val > best_val:
                best_val = val
                best_seq = seq
                improved = True
                break
            else:
                best_seq[i], best_seq[i+1] = best_seq[i+1], best_seq[i]
        if not improved:
            used_set = set(best_seq)
            unused = [fid for _, fid, _, _ in facto if fid not in used_set]
            random.shuffle(unused)
            for fid in unused[:10]:
                for pos in range(len(best_seq) + 1):
                    cand = best_seq[:pos] + [fid] + best_seq[pos:]
                    val, seq = evaluate_solution(cand)
                    if val > best_val:
                        best_val = val
                        best_seq = seq
                        improved = True
                        break
                if improved:
                    break
    ti = 0
    answer = []
    used_p = [False] * N
    for fid in best_seq:
        if fid not in facto_by_id:
            continue
        T, K, ax = facto_by_id[fid]
        if ti + T >= D:
            continue
        ti += T
        remaining_days = D - ti
        p = min(K, remaining_days)
        chosen = []
        for x in ax:
            if not used_p[x]:
                chosen.append(x)
                used_p[x] = True
                if len(chosen) == p:
                    break
        answer.append((fid, chosen))
    print(len(answer))
    for fid, pickaxes in answer:
        print(fid, len(pickaxes), *pickaxes)
solve()

