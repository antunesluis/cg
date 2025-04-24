#include "ImageLoaderUI.h"
#include "Config.h"
#include "gl_canvas2d.h"
#include <algorithm>
#include <dirent.h>

ImageLoaderUI::ImageLoaderUI() : width(600), height(400) { updateImageList(); }

void ImageLoaderUI::setVisible(bool visible, Layer *targetLayer)
{
    this->visible = visible;
    this->targetLayer = targetLayer;
    if (visible) {
        posX = (config::screen::width - width) / 2;
        posY = (config::screen::height - height) / 2;
        updateImageList();
    }
}

void ImageLoaderUI::updateImageList()
{
    imagePaths.clear();
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("./src/images/")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string name = ent->d_name;
            if (name.size() > 4 && name.substr(name.size() - 4) == ".bmp") {
                imagePaths.push_back("./src/images/" + name);
            }
        }
        closedir(dir);
        std::sort(imagePaths.begin(), imagePaths.end());
    }
}

void ImageLoaderUI::render(int mouseX, int mouseY)
{
    if (!visible)
        return;

    // Fundo do modal
    CV::color(0.2f, 0.2f, 0.25f);
    CV::rectFill(posX, posY, posX + width, posY + height);

    // Borda
    CV::color(0.4f, 0.4f, 0.5f);
    CV::rect(posX, posY, posX + width, posY + height);

    // Título
    CV::color(1, 1, 1);
    CV::text(posX + 20, posY + 30, "Selecione uma imagem:");

    // Lista de imagens
    float itemHeight = 30;
    float startY = posY + 50;

    for (size_t i = 0; i < imagePaths.size(); i++) {
        float y = startY + i * itemHeight;

        // Destaque no hover
        if (mouseX >= posX && mouseX <= posX + width && mouseY >= y && mouseY <= y + itemHeight) {
            CV::color(0.3f, 0.3f, 0.4f);
            CV::rectFill(posX, y, posX + width, y + itemHeight);
        }

        // Texto do item
        CV::color(1, 1, 1);
        size_t lastSlash = imagePaths[i].find_last_of("/\\");
        std::string fileName = imagePaths[i].substr(lastSlash + 1);
        CV::text(posX + 20, y + 20, fileName.c_str());
    }
}

bool ImageLoaderUI::handleMouse(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if (!visible)
        return false;

    if (button == 0 && state == 0) {
        float itemHeight = 30;
        float startY = posY + 50;

        for (size_t i = 0; i < imagePaths.size(); i++) {
            float itemY = startY + i * itemHeight;
            if (x >= posX && x <= posX + width && y >= itemY && y <= itemY + itemHeight) {

                if (targetLayer != nullptr) {
                    targetLayer->loadImage(imagePaths[i].c_str());
                    imageSelected = true; // Marca que uma imagem foi selecionada
                    visible = false;      // Fecha o menu imediatamente
                    return true;
                }
            }
        }

        // Fecha o menu se clicar fora
        if (x < posX || x > posX + width || y < posY || y > posY + height) {
            visible = false;
        }
    }
    return false;
}
