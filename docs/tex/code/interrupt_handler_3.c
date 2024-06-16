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
