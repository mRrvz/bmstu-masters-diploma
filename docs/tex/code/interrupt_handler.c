static void enter_exception64(struct kvm_vcpu *vcpu,
			      unsigned long target_mode,
			      enum exception_type type)
{
	unsigned long sctlr, vbar, old, new, mode;
	u64 exc_offset;

	mode = *vcpu_cpsr(vcpu) & (PSR_MODE_MASK | PSR_MODE32_BIT);

	if (mode == target_mode)
		exc_offset = CURRENT_EL_SP_ELx_VECTOR;
	else if ((mode | PSR_MODE_THREAD_BIT) == target_mode)
		exc_offset = CURRENT_EL_SP_EL0_VECTOR;
	else if (!(mode & PSR_MODE32_BIT))
		exc_offset = LOWER_EL_AArch64_VECTOR;
	else
		exc_offset = LOWER_EL_AArch32_VECTOR;

	switch (target_mode) {
	case PSR_MODE_EL1h:
		vbar = __vcpu_read_sys_reg(vcpu, VBAR_EL1);
		sctlr = __vcpu_read_sys_reg(vcpu, SCTLR_EL1);
		__vcpu_write_sys_reg(vcpu, *vcpu_pc(vcpu), ELR_EL1);
		break;
	case PSR_MODE_EL2h:
		vbar = __vcpu_read_sys_reg(vcpu, VBAR_EL2);
		sctlr = __vcpu_read_sys_reg(vcpu, SCTLR_EL2);
		__vcpu_write_sys_reg(vcpu, *vcpu_pc(vcpu), ELR_EL2);
		break;
	default:
		BUG();
	}

	*vcpu_pc(vcpu) = vbar + exc_offset + type;

	old = *vcpu_cpsr(vcpu);

	if (need_intercepted(old)) {
		new = do_optee_handle_action(target_mode, type, old);
	} else {
		new = 0;

		new |= (old & PSR_N_BIT);
		new |= (old & PSR_Z_BIT);
		new |= (old & PSR_C_BIT);
		new |= (old & PSR_V_BIT);

		if (kvm_has_mte(kern_hyp_va(vcpu->kvm)))
			new |= PSR_TCO_BIT;

		new |= (old & PSR_DIT_BIT);

		new |= (old & PSR_PAN_BIT);
		if (!(sctlr & SCTLR_EL1_SPAN))
			new |= PSR_PAN_BIT;

		if (sctlr & SCTLR_ELx_DSSBS)
			new |= PSR_SSBS_BIT;

		new |= PSR_D_BIT;
		new |= PSR_A_BIT;
		new |= PSR_I_BIT;
		new |= PSR_F_BIT;

		new |= target_mode;
	}

	*vcpu_cpsr(vcpu) = new;
	__vcpu_write_spsr(vcpu, target_mode, old);
}
