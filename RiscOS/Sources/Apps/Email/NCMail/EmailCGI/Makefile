#  Makefile for Email Client CGI/Perl configuration scripts
# -========================================================-
#
# Initial version: 28th Feb 2002, SAR

COMPONENT = EmailCGI

IFTHERE    = IfThere
IFTHEN     = Then
IFELSE     = Else

LOCALTRANS = ${COMPONENT}TransRes
TRANSFLAGS = ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë" -m ${LOCALTRANS}:Mappings

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

ram: translate vars dirs
	${CHMOD} -R 0755 ${INSTDIR}
	@echo Component ${COMPONENT} built (ram).

install: translate vars dirs
	${CHMOD} -R 0755 ${INSTDIR}
	@echo Component ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INSTDIR}

vars:
	@Set ${LOCALTRANS}$Path Translate.${UserIF}.${Locale}.,Translate.${UserIF}.UK.

translate:
	${IFTHERE} ${LOCALTRANS}:accounts/pl  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:accounts/pl -o ${INSTDIR}.accounts/pl
	${IFTHERE} ${LOCALTRANS}:create/pl    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:create/pl   -o ${INSTDIR}.create/pl
	${IFTHERE} ${LOCALTRANS}:modify/pl    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:modify/pl   -o ${INSTDIR}.modify/pl
	${IFTHERE} ${LOCALTRANS}:m_submit/pl  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:m_submit/pl -o ${INSTDIR}.m_submit/pl
	${IFTHERE} ${LOCALTRANS}:Accounts_H   ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:Accounts_H -o ${INSTDIR}.Accounts_H
	${IFTHERE} ${LOCALTRANS}:Complete_H  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:Complete_H -o ${INSTDIR}.Complete_H
	${IFTHERE} ${LOCALTRANS}:Create_H  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:Create_H -o ${INSTDIR}.Create_H
	${IFTHERE} ${LOCALTRANS}:Modify_H  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:Modify_H -o ${INSTDIR}.Modify_H
	${IFTHERE} ${LOCALTRANS}:ServerTools/html  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i ${LOCALTRANS}:ServerTools/html -o ${INSTDIR}.ServerTools/html

# Dynamic dependencies:
