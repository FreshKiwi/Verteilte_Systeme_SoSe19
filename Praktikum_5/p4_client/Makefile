ALL = pub_sub_client
RPCGEN = rpcgen

all: $(ALL)

pub_sub_clnt.c: pub_sub.x
	$(RPCGEN) pub_sub.x
	$(RM) pub_sub_svc.c


pub_sub_client: pub_sub_clnt.c pub_sub_clnt_proc.c pub_sub_xdr.c pub_sub.h return_codes.h
	gcc -g -o pub_sub_client pub_sub_clnt.c pub_sub_clnt_proc.c pub_sub_xdr.c 

clean:
	$(RM) *.o  pub_sub_xdr.c pub_sub_clnt.c pub_sub_client
