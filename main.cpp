#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

bool isPortAvailable(int port) {
    // Création du socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Erreur: impossible de créer un socket." << std::endl;
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // Accepter les connexions sur toutes les interfaces
    addr.sin_port = htons(port); // Convertit le port en format réseau

    // Essayer de binder le socket au port
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sock); // Libérer le socket
        return false;
    }
    close(sock);
    return true;
}

int	main(int ac, char **av)
{
	if (ac != 3)
		return (std::cerr << "Wrong arguments" << std::endl, 2);
	(void)av[2];
	int port = std::stoi(av[1]);
	if (port > 1024 && port < 49152 && isPortAvailable(port))
	{
        std::cout << "Le port " << port << " est disponible." << std::endl;
    } else {
        std::cout << "Le port " << port << " est indisponible !" << std::endl;
    }
    return 0;
}