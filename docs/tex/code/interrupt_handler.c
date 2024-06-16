static void enter_exception64(struct kvm_vcpu *vcpu,
			      unsigned long target_mode,
			      enum exception_type type)
{
	unsigned long sctlr, vbar, old, new, mode;
	u64 exc_offset;

	mode = *vcpu_cpsr(vcpu) & PSR_MODE_MASK;
