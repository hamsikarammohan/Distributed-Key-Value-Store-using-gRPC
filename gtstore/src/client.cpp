#include "gtstore.hpp"

void GTStoreClient::init(int id) {

		cout << "Inside GTStoreClient::init() for client " << id << "\n";
		client_id = id;
}

val_t GTStoreClient::get(string key) {

		cout << "Inside GTStoreClient::get() for client: " << client_id << " key: " << key << "\n";
		val_t value;
		// Get the value!
		return value;
}

bool GTStoreClient::put(string key, val_t value) {

		string print_value = "";
		for (uint i = 0; i < value.size(); i++) {
				print_value += value[i] + " ";
		}
		cout << "Inside GTStoreClient::put() for client: " << client_id << " key: " << key << " value: " << print_value << "\n";
		// Put the value!
		return true;
}

void GTStoreClient::finalize() {

		cout << "Inside GTStoreClient::finalize() for client " << client_id << "\n";
}
