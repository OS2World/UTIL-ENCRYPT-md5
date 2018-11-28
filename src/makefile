#
# Makefile for MD5 digest utility
#
# Bob Eager   October 2000
#
# Product name
#
PRODUCT		= md5
#
# Target directory
#
TARGET		= f:\binp
#
# Compiler setup
#
CC		= icc
!IFDEF DEBUG
DBUG		= -DDEBUG
!ELSE
DBUG		=
!ENDIF
!IFDEF	PROD
CFLAGS		= $(DBUG) -Fi -G4 -Gn -O -Q -Se -Si -Tl30
CLIB		= cppos30.lib
!ELSE
CFLAGS		= $(DBUG) -Fi -G4 -Gn -Q -Se -Si -Ti -Tl30 -Tm -Tx
CLIB		= cppos30.lib
!ENDIF
#
# Names of library files
#
LIBS =		$(CLIB) os2386.lib
#
# Names of object files
#
OBJ =		md5.obj md5c.obj md5hl.obj
#
# Other files
#
DEF =		$(PRODUCT).def
LNK =		$(PRODUCT).lnk
#
# Final executable file
#
EXE =		$(PRODUCT).exe
#
# Distribution
#
DIST =		dist.zip
#
#-----------------------------------------------------------------------------
#
$(EXE):		$(OBJ) $(LNK) $(DEF)
!IFDEF	PROD
		ilink /nologo /exepack:2 @$(LNK)
!ELSE
		ilink /debug /nobrowse /nologo @$(LNK)
!ENDIF
#
# Object files
#
md5.obj:	md5.c md5.h
#
md5c.obj:	md5c.c md5.h
#
md5hl.obj:	md5hl.c md5.h
#
# Linker response file. Rebuild if makefile changes
#
$(LNK):		makefile
		@if exist $(LNK) erase $(LNK)
		@echo /map:$(PRODUCT) >> $(LNK)
		@echo /out:$(PRODUCT) >> $(LNK)
		@echo $(OBJ) >> $(LNK)
		@echo $(LIBS) >> $(LNK)
		@echo $(DEF) >> $(LNK)
#
clean:		
		-erase $(OBJ) $(LNK) $(PRODUCT).map csetc.pch
#
install:	$(EXE)
		@copy $(EXE) $(TARGET) > nul
#
dist:		$(EXE) readme.txt
		zip -9 -j $(DIST) $**
#
# End of makefile for MD5 digest utility
#
