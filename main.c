#include <stdio.h>
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Variável global para armazenar o identificador do controle de edição
HWND hEdit;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Registrar a classe da janela
    const char className[] = "NotepadApp";
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "Erro ao registrar a classe da janela!", "Erro", MB_ICONERROR);
        return 1;
    }

    // Criar a janela
    HWND hwnd = CreateWindowEx(
        0,
        className,
        "Bloco de Notas",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Erro ao criar a janela!", "Erro", MB_ICONERROR);
        return 1;
    }

    // Criar o controle de edição
    hEdit = CreateWindowEx(
        0,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
        0, 0, 624, 442,
        hwnd,
        NULL,
        hInstance,
        NULL);

    if (hEdit == NULL)
    {
        MessageBox(NULL, "Erro ao criar o controle de edição!", "Erro", MB_ICONERROR);
        return 1;
    }

    // Mostrar e atualizar a janela
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Loop da mensagem
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        // Aqui você pode criar os controles da janela, como um controle de edição para o texto.
        break;
    case WM_CLOSE:
    {
        // Antes de fechar a janela, vamos salvar o conteúdo do controle de edição em um arquivo .txt

        // Obter o tamanho do texto no controle de edição
        int length = GetWindowTextLength(hEdit);

        if (length > 0)
        {
            // Alocar memória para armazenar o texto
            char* buffer = (char*)malloc(length + 1);
            if (buffer)
            {
                // Obter o texto do controle de edição
                GetWindowText(hEdit, buffer, length + 1);

                // Abrir o arquivo para escrita
                FILE* file = fopen("texto_salvo.txt", "w");
                if (file)
                {
                    // Escrever o texto no arquivo
                    fwrite(buffer, 1, length, file);
                    fclose(file);
                }

                free(buffer);
            }
        }

        // Fechar a janela
        DestroyWindow(hwnd);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
