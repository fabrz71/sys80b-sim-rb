.PHONY: clean All

All:
	@echo "----------Building project:[ sys80b-sim-rb - Debug_Win32 ]----------"
	@"$(MAKE)" -f  "sys80b-sim-rb.mk"
clean:
	@echo "----------Cleaning project:[ sys80b-sim-rb - Debug_Win32 ]----------"
	@"$(MAKE)" -f  "sys80b-sim-rb.mk" clean
