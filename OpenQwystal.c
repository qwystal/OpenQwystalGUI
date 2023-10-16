/*
gcc --% -IC:/msys64/mingw64/bin/../include/gtk-4.0 -IC:/msys64/mingw64/bin/../include/pango-1.0 -IC:/msys64/mingw64/bin/../include/gdk-pixbuf-2.0 -IC:/msys64/mingw64/bin/../include/cairo -IC:/msys64/mingw64/bin/../include/graphene-1.0 -IC:/msys64/mingw64/bin/../lib/graphene-1.0/include -mfpmath=sse -msse -msse2 -IC:/msys64/mingw64/bin/../include -IC:/msys64/mingw64/bin/../include/harfbuzz -IC:/msys64/mingw64/bin/../include/freetype2 -IC:/msys64/mingw64/bin/../include/glib-2.0 -IC:/msys64/mingw64/bin/../lib/glib-2.0/include -IC:/msys64/mingw64/bin/../include/webp -DLIBDEFLATE_DLL -IC:/msys64/mingw64/bin/../include/pixman-1 -IC:/msys64/mingw64/bin/../include/libpng16 -IC:/msys64/mingw64/bin/../include/fribidi .\OpenQwystal.c -o .\OpenQwystal.exe -LC:/msys64/mingw64/bin/../lib -lgtk-4 -lpangowin32-1.0 -lharfbuzz -lpangocairo-1.0 -lpango-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lcairo -lgraphene-1.0 -lgio-2.0 -lglib-2.0 -lintl -lgobject-2.0 -lcurl
*/

#include "include.h"
#include "get_request.c"
#include "window.c"

#define APIKEY_MAX_LENGTH 128

void AssertNotNULL(const void *ptr) {
    if (ptr == NULL) {
        printf("Not enough memory\n");
        exit(1);
    }
}

char *GetCredentials() { // make storing the apikey more secure
    char *apikey = (char *)malloc(APIKEY_MAX_LENGTH + 1);
    char *credentials = (char *)malloc(APIKEY_MAX_LENGTH + 7);
    if (apikey == NULL || credentials == NULL)
    {
        return NULL;
    }
    

    if (apikey == NULL) {
        return NULL;
    }

    printf("Enter your API key: ");
    fgets(apikey, APIKEY_MAX_LENGTH, stdin);

    // remove trailing newline
    apikey[strcspn(apikey, "\n")] = 0;

    // add username and apikey to apikey
    snprintf(credentials, APIKEY_MAX_LENGTH + 7, "apikey:%s", apikey);
    return credentials;
}

char *GetAddress() {
    char *address = (char *)malloc(256);
    if (address == NULL) {
        return NULL;
    }
    
    printf("Enter the Webaddress of the project: ");
    fgets(address, 256, stdin);
    // If there is no http:// or https:// in the address, add https://
    if (strncmp(address, "https://", 8) != 0) {
        char *temp = (char *)malloc(256);
        if (temp == NULL) {
            return NULL;
        }
        snprintf(temp, 256, "https://%s", address);
        address = temp;
    }

     // remove trailing newline
    address[strcspn(address, "\n")] = 0;
    
    // remove everything after the third slash if there is no third slash add one at the end
    int slashCount = 0;
    
    for (int i = 0; i < strlen(address); i++) {
        if (address[i] == '/') {
            slashCount++;
        }
        
        if (slashCount == 3) {
            // Found the third slash, remove everything after it
            address[i + 1] = '\0';
            i = strlen(address);
        }
    }
    
    // If there are less than three slashes, add one at the end
    if (slashCount < 3) {
        strcat(address, "/");
    }

    // add page to address
    snprintf(address, 256, "%s/api/v3/", address);

    return address;
}

char *AddAddress(const char *address, char *page) { // add something to the address. for example: the address is op.example.com and the page is users/me, the result will be op.example.com/users/me
    char *temp = (char *)malloc(2048);
    if (temp == NULL) {
        return NULL;
    }
    snprintf(temp, 2048, "%s%s", address, page);
    return temp;
}

int main(int argc, char **argv) {
    initializeWindow();
    const char *address = GetAddress();
    AssertNotNULL(address);

    const char *credentials = GetCredentials();
    AssertNotNULL(credentials);

    cJSON *json = GetRequest(AddAddress(address, "users/me"), credentials);

    const cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
    initializeWindow();
        
    if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        printf("Welcome, %s!\n", name->valuestring);
    }
    cJSON_Delete(json);

    json = GetRequest(AddAddress(address, ""), credentials);
    cJSON *openProject = cJSON_GetObjectItemCaseSensitive(json, "instanceName");
    printf("You are using %s\n", openProject->valuestring);

    // Create a new application
    
    getchar();

    return 0;
}