#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 

struct SubnetInfo {
    char ip_entered[16];
    int cidr;
    char netmask[16];
    char netmask_hex[12];
    char wildcard[16];
    char network_address[16];
    char broadcast_address[16];
    int usable_ips;
    char first_usable_ip[16];
    char last_usable_ip[16];
};

// Função para validar se o IP é um IPv4 válido
int is_valid_ipv4(const char *ip) {
    char ip_copy[16];
    strcpy(ip_copy, ip);  // Faz uma cópia para não modificar a string original

    // Verifica o comprimento básico da string
    if (strlen(ip_copy) < 7 || strlen(ip_copy) > 15) {  // Ex.: "1.2.3.4" (7 chars) até "255.255.255.255" (15 chars)
        return 0;
    }

    // Divide o IP em octetos usando o ponto como delimitador
    char *token = strtok(ip_copy, ".");
    int octets_count = 0;
    while (token != NULL) {
        // Conta os octetos
        octets_count++;

        // Verifica se há mais de 4 octetos
        if (octets_count > 4) {
            return 0;
        }

        // Verifica se o octeto contém apenas dígitos
        for (int i = 0; i < strlen(token); i++) {
            if (!isdigit(token[i])) {
                return 0;
            }
        }

        // Verifica zeros à esquerda (ex.: "01" ou "001" é inválido)
        if (strlen(token) > 1 && token[0] == '0') {
            return 0;
        }

        // Converte o octeto para inteiro e verifica o intervalo
        int octet = atoi(token);
        if (octet < 0 || octet > 255) {
            return 0;
        }

        // Pega o próximo octeto
        token = strtok(NULL, ".");
    }

    // Verifica se o número de octetos é exatamente 4
    if (octets_count != 4) {
        return 0;
    }

    return 1;  // IP válido
}

int main(int argc, char *argv[]) {
    struct SubnetInfo subnet;

    // Inicializa os campos
    subnet.ip_entered[0] = '\0';
    subnet.netmask[0] = '\0';
    subnet.netmask_hex[0] = '\0';
    subnet.wildcard[0] = '\0';
    subnet.network_address[0] = '\0';
    subnet.broadcast_address[0] = '\0';
    subnet.first_usable_ip[0] = '\0';
    subnet.last_usable_ip[0] = '\0';
    subnet.cidr = 0;
    subnet.usable_ips = 0;

    // Valida o número de argumentos
    if (argc != 2) {
        printf("Uso: %s <IP/CIDR>\n", argv[0]);
        printf("Exemplo: %s 192.168.23.2/24\n", argv[0]);
        return 1;
    }

    // Processa o argumento (IP/CIDR)
    char *slash = strchr(argv[1], '/');
    if (slash == NULL) {
        printf("Erro: Formato inválido. Use IP/CIDR (ex.: 192.168.23.2/24)\n");
        return 1;
    }
    *slash = '\0';  // Divide a string substituindo a barra por um terminador nulo
    strcpy(subnet.ip_entered, argv[1]);  // Copia o IP
    subnet.cidr = atoi(slash + 1);  // Converte o CIDR para inteiro

    // Valida o IP
    if (!is_valid_ipv4(subnet.ip_entered)) {
        printf("Erro: Endereço IP inválido. Deve ser um IPv4 válido (ex.: 192.168.23.2)\n");
        return 1;
    }

    // Valida o CIDR
    if (subnet.cidr < 0 || subnet.cidr > 32) {
        printf("Erro: CIDR deve estar entre 0 e 32.\n");
        return 1;
    }

    // Imprime os dados
    printf("\nTCP/IP NETWORK INFORMATION\n");
    printf("IP Entered = ........: %s\n", subnet.ip_entered);
    printf("CIDR = .............: /%d\n", subnet.cidr);
    printf("Netmask = ..........: %s\n", subnet.netmask);
    printf("Netmask (hex) = ....: %s\n", subnet.netmask_hex);
    printf("Wildcard Bits = ....: %s\n", subnet.wildcard);
    printf("Network Address = ..: %s\n", subnet.network_address);
    printf("Broadcast Address = : %s\n", subnet.broadcast_address);
    printf("Usable IP Addresses : %d\n", subnet.usable_ips);
    printf("First Usable IP = ..: %s\n", subnet.first_usable_ip);
    printf("Last Usable IP = ...: %s\n", subnet.last_usable_ip);

    return 0;
}