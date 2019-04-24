ALL = pub_sub_deliv_receiver
RPCGEN = rpcgen

all: $(ALL)

pub_sub_deliv_svc.c: pub_sub_deliv.x
	$(RPCGEN) pub_sub_deliv.x
	$(RM) pub_sub_deliv_clnt.c
	$(RM) pub_sub_deliv_svc.c
	$(RPCGEN) -s tcp -o pub_sub_deliv_svc.c pub_sub_deliv.x
	

pub_sub_deliv_receiver: pub_sub_deliv_svc.c pub_sub_deliv_proc.c pub_sub_deliv_xdr.c pub_sub_deliv.h return_codes.h
	gcc -g -o pub_sub_deliv_receiver pub_sub_deliv_svc.c pub_sub_deliv_proc.c pub_sub_deliv_xdr.c 

clean:
	$(RM) *.o  pub_sub_deliv_svc.c pub_sub_deliv.h pub_sub_deliv_xdr.c pub_sub_deliv_receiver
