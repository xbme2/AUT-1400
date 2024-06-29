#include "client.h"

Client::Client(std::string id, const Server& server):id(id),server(&server){
    crypto::generate_key(this->public_key, this->private_key);
}
std::string Client::get_id() const{
    return this->id;
}
std::string Client::get_publickey() const{
    return this->public_key;
}
double Client::get_wallet() const{
    return server->get_wallet(this->id);
}
std::string Client::sign(std::string txt) const{
    return  crypto::signMessage(private_key, txt);
    // return authentic = crypto::verifySignature(public_key, "my data", signature);
}
bool Client::transfer_money(std::string receiver, double value){
    std::string trx=this->id+"-"+receiver+"-"+std::to_string(value);
    return server->add_pending_trx(trx,sign(trx));
}
size_t Client::generate_nonce(){
    static std::default_random_engine e;
    static std::uniform_int_distribution u(0, INT_MAX);
    return u(e);
}