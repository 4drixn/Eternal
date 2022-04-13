.code
; void unload_and_free(void* base, std::size_t size, void* zero_memory, void* return_addr);
unload_and_free proc
	push r9			; return to whatever was calling the function that called this routine.
	jmp r8			; address of zero memory
unload_and_free endp
public unload_and_free
end
