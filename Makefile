OBJNAME = my_objdump

NMNAME = my_nm

RM = rm -f

all: nm objdump

nm:
	make -C nm && mv nm/$(NMNAME) .

objdump:
	make -C objdump && mv objdump/$(OBJNAME) .

clean:
	make fclean -C nm
	make fclean -C objdump

fclean: clean
	$(RM) $(OBJNAME) $(NMNAME)

re: fclean all

.PHONY: all nm objdump clean fclean re
