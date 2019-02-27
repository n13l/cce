$(o)/%.o: %.c
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -o $@ -c $< 
	$(M)CC   $<

$(o)/%.o: %.cc
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CXX) $(CXXFLAGS) -o $@ -c $< 
	$(M)CX    $<

$(o)/%: $(o)/%.o
	$(Q)mkdir -p $(dir $@)
	$(M)LD   $(patsubst $(o)/%,%,$@)
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS)

%.a:
	$(Q)mkdir -p $(dir $@)
	$(M)AR   $(patsubst $(o)/%,%,$@)
	$(Q)rm -f $@                                                            
	$(Q)ar rcs $@ $^


