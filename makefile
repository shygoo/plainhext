CC=gcc
CFLAGS=-Wall -O3 -s
LDFLAGS=

SDIR=src
ODIR=obj
BDIR=bin

SFILES=$(wildcard $(SDIR)/*.c)
OFILES=$(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(SFILES))

plainhext: $(OFILES) | $(BDIR)
	$(CC) $(CFLAGS) $(OFILES) -o $(BDIR)/plainhext

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(ODIR):
	mkdir $(ODIR)
	
$(BDIR):
	mkdir $(BDIR)
	
.PHONY: clean
clean:
	rm -rf $(ODIR)
	rm -rf $(BDIR)