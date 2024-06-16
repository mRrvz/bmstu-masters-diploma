	return ret;
}

static int sec_vm_pgtable_walk(
	struct sec_vm_pgtable_walk_data *data,
	struct sec_vm_pgtable_mm_ops *mm_ops,
	sec_vm_pteref_t pgtable, s8 level)
{
	u32 idx;
	int ret = 0;

	if (level < sec_vm_PGTABLE_FIRST_LEVEL ||
			 level > sec_vm_PGTABLE_LAST_LEVEL)
		return -EINVAL;

	for (idx = sec_vm_pgtable_idx(data, level);
		idx < PTRS_PER_PTE; ++idx) {
		sec_vm_pteref_t pteref = &pgtable[idx];

		ret = __sec_vm_pgtable_visit(data, mm_ops, 
					     pteref, level);
	}

	return ret;
}
