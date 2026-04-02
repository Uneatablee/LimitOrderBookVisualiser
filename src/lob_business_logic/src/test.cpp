#include <iostream>
#include <zmq.h>

void sprawdz_zmq() {
    // 1. Sprawdzenie wersji
    int major, minor, patch;
    zmq_version(&major, &minor, &patch);
    std::cout << "Wersja ZeroMQ: " << major << "." << minor << "." << patch << "\n";

    void* context = zmq_ctx_new();
    if (context != nullptr) {
        std::cout << "Kontekst ZeroMQ utworzony pomyslnie!\n";
        zmq_ctx_destroy(context);
    } else {
        std::cerr << "Blad podczas tworzenia kontekstu ZeroMQ!\n";
    }
}