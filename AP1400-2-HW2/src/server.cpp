#include "server.h"
#include <random>

std::vector<std::string> pending_trxs;

Server ::Server(){
    clients = map<std::shared_ptr<Client>, double>{};
}
std::map<std::shared_ptr<Client>, double>& Server::get_map() {
    return clients;
}
std::shared_ptr<Client> Server::add_client(std::string id){
    if(this->get_client(id)!=nullptr){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1000, 9999);
        int randNum = dis(gen);
        id += std::to_string(randNum);
    }
    shared_ptr added_client=std::make_shared<Client>(id,*this);
    clients.insert(std::make_pair(added_client,5));
    return added_client;
}
std::shared_ptr<Client> Server::get_client(std::string id) const{
    for(const auto &client : clients){
        if(client.first->get_id()==id)
            return client.first;
    }
    return nullptr;
}
double Server::get_wallet(std::string id) const{
    for(const auto &client : clients){
        if(client.first->get_id()==id)
            return client.second;
    }
    return 0.0;
}
bool Server::parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value) {
    std::string pattern = "(\\w+)-(\\w+)-(\\d+\\.\\d+)";
	std::regex r(pattern);
	std::smatch results;
	if (std::regex_match(trx, results, r)) {
        sender=results.str(1);
        receiver=results.str(2);
        value=std::stod(results.str(3));
        return true;
    }
    else{
        throw std::runtime_error("this parse_trx not right!");
    }
    return false;
}
bool Server::add_pending_trx(std::string trx, std::string signature) const{
    std::string sender,  receiver ; double value;
    if(!parse_trx(trx,sender,receiver,value))
        return false;
    auto m_sender=get_client(sender);
    auto m_receiver=get_client(receiver);
    if( m_receiver==nullptr)
        return false;
    
    bool authentic = crypto::verifySignature(m_sender->get_publickey(), trx, signature);
    if(!authentic||get_wallet(sender)<value)
        return false;
    
    pending_trxs.push_back(trx);
    //std::cout<<pending_trxs.size()<<std::endl;

    return true;
}
size_t Server::mine(){
    std::string mempool{};
    for(const auto &trx:pending_trxs){
        mempool+=trx;
    }
    size_t nonce = 0;
    bool ismined=false;
    while(!ismined){
        for( auto & client:clients){
            nonce=client.first->generate_nonce();
            if(crypto::sha256(mempool + std::to_string(nonce)).substr(0, 10).find("000") != std::string::npos){
                ismined=true;
                client.second+=6.25;
                for(const auto &trx:pending_trxs){
                    std::string sender, receiver;double value;
					Server::parse_trx(trx, sender, receiver, value);
					clients[get_client(sender)] -= value;
					clients[get_client(receiver)] += value;
                }
                pending_trxs.clear();
                return nonce;
            }
        }
    }
    return nonce;
}


void  show_wallets(  Server& server)
 {
 	std::cout << std::string(20, '*') << std::endl;
 	for(const auto client: server.get_map())
 		std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
 	std::cout << std::string(20, '*') << std::endl;
}