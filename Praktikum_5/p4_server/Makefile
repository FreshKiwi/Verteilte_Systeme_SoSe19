ALL = p4_server
RPCGEN = rpcgen

all: $(ALL)

pub_sub_svc.c: pub_sub.x
	$(RPCGEN) pub_sub.x
	$(RM) pub_sub_svc.c
	$(RPCGEN) -s tcp -o pub_sub_svc.c pub_sub.x
	$(RM) pub_sub_clnt.c
	$(RPCGEN) pub_sub_deliv.x
	$(RM) pub_sub_deliv_svc.c


p4_server: pub_sub_svc.c pub_sub_server_proc.c pub_sub_deliv_clnt.c pub_sub_xdr.c pub_sub_deliv_xdr.c pub_sub.h pub_sub_deliv.h return_codes.h
	gcc -g -o p4_server pub_sub_deliv_clnt.c pub_sub_deliv_xdr.c pub_sub_xdr.c pub_sub_server_proc.c pub_sub_svc.c

clean:
	$(RM) *.o  pub_sub_xdr.c pub_sub_svc.c pub_sub_deliv_clnt.c pub_sub.h pub_sub_deliv_xdr.c pub_sub_deliv.h p4_server
