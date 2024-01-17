#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transforms2D.h"
#include "lab_m1/Tema1/objects2D.h"

using namespace std;
using namespace m1;

vector<glm::mat3> hexagonModelMatrices;
vector<int> hexagonTypes; // 1,2,3,4
vector<int> hexagonLevels; // 0,1,2 (de jos in sus)
vector<bool> hexagonRemoved(10000, false);
vector<bool> toCountHexagon(10000, false);
vector<int> hexagonHits;
vector<bool> shouldScale(10000, false);
vector<float> scaleHexagon(10000, 1.0f);

vector<glm::mat3> starModelMatrices; // stelele aparute random pe ecran

vector<glm::mat3> projectileMatrices;
vector<int> projectileTypes;
vector<bool> projectileRemoved(100000, false);

vector<glm::mat3> addedStarsMatrices; // stelele adunate (initial 5)

vector<vector<bool>> collisionProcessed(10000, std::vector<bool>(100000, false)); // tine minte daca s a facut coliziune intre hexagon si proiectil
vector<vector<bool> > collisionProcessedRhombus(10000, vector<bool>(100000, false)); // intre hexagon si romb

vector<glm::mat3> rhombusMatrices;
vector<int> rhombusSquare; // in ce patrat se afla rombul plasat
vector<int> rhombusTypes;
vector<bool> rhombusRemoved(100000, false);
vector<float> scaleRhombus(10000, 1.0f);
vector<bool> rhombusClicked(100000, false);


// verifica coliziunea intre 2 obiecte
bool isCircleCollision(const glm::vec3& center1, float radius1, const glm::vec3& center2, float radius2) {
    float distanceSquared = glm::distance2(center1, center2);
    float radiusSum = radius1 + radius2;
    return distanceSquared < (radiusSum * radiusSum);
}

// verifica daca cursorul se apasa in interiorul unui patrat (cel ce cuprinde rombul)
bool isPointInsideSquare(const glm::vec3& point, glm::vec3& squareCenter, float squareSide) {
    squareCenter = glm::vec3(squareCenter.x + 60, squareCenter.y + 60, 0); // centrul cercului, inainte era coltul
	float halfSide = squareSide / 2;
	return (point.x >= squareCenter.x - halfSide && point.x <= squareCenter.x + halfSide &&
        		point.y >= squareCenter.y - halfSide && point.y <= squareCenter.y + halfSide);
}

// verifica daca cursorul se apasa in interiorul unei stele
bool isPointInsideStar(const glm::vec3& point, glm::vec3& starCenter, float Side) {
    starCenter = glm::vec3(starCenter.x, starCenter.y, 0); // centrul cercului, inainte era coltul
    float halfSide = Side / 2;
    return (point.x >= starCenter.x - halfSide && point.x <= starCenter.x + halfSide &&
        point.y >= starCenter.y - halfSide && point.y <= starCenter.y + halfSide);
}

// genereaza proiectilele daca trec 3 secunde de la precedenta. Se apeleaza cand e rombul specific plasat si daca sunt hexagoanele specifice pe linie
void sendProjectiles(int x, int y, int type) {
    glm::mat3 projectileMatrix = glm::mat3(1);
    projectileMatrix *= transforms2D::Translate(x, y);
    projectileMatrices.push_back(projectileMatrix);
    projectileTypes.push_back(type);
}

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    Mesh* rectangle = objects2D::CreateRectangle("rectangle", glm::vec3(10, 20, 0), 60, 440, glm::vec3(0.7f, 0.2f, 0.1f));
    AddMeshToList(rectangle);

    Mesh* square1 = objects2D::CreateSquare("square1", glm::vec3(100, 20, 0), squareSide, squareColor, true);
    AddMeshToList(square1);

    Mesh* square2 = objects2D::CreateSquare("square2", glm::vec3(260,20,0), squareSide, squareColor, true);
    AddMeshToList(square2);

    Mesh* square3 = objects2D::CreateSquare("square3", glm::vec3(420, 20, 0), squareSide, squareColor, true);
    AddMeshToList(square3);

    Mesh* square4 = objects2D::CreateSquare("square4", glm::vec3(100, 180, 0), squareSide, squareColor, true);
    AddMeshToList(square4);

    Mesh* square5 = objects2D::CreateSquare("square5", glm::vec3(260, 180, 0), squareSide, squareColor, true);
    AddMeshToList(square5);

    Mesh* square6 = objects2D::CreateSquare("square6", glm::vec3(420, 180, 0), squareSide, squareColor, true);
    AddMeshToList(square6);

    Mesh* square7 = objects2D::CreateSquare("square7", glm::vec3(100, 340, 0), squareSide, squareColor, true);
    AddMeshToList(square7);

    Mesh* square8 = objects2D::CreateSquare("square8", glm::vec3(260, 340, 0), squareSide, squareColor, true);
    AddMeshToList(square8);

    Mesh* square9 = objects2D::CreateSquare("square9", glm::vec3(420, 340, 0), squareSide, squareColor, true);
    AddMeshToList(square9);

    Mesh* frame1 = objects2D::CreateSquare("frame1", glm::vec3(60, 580, 0), squareSide, glm::vec3(0, 0, 0), false);
    AddMeshToList(frame1);

    Mesh* frame2 = objects2D::CreateSquare("frame2", glm::vec3(260, 580, 0), squareSide, glm::vec3(0, 0, 0), false);
    AddMeshToList(frame2);

    Mesh* frame3 = objects2D::CreateSquare("frame3", glm::vec3(460, 580, 0), squareSide, glm::vec3(0, 0, 0), false);
    AddMeshToList(frame3);

    Mesh* frame4 = objects2D::CreateSquare("frame4", glm::vec3(660, 580, 0), squareSide, glm::vec3(0, 0, 0), false);
    AddMeshToList(frame4);

    Mesh* rhombus1 = objects2D::CreateRhombus("rhombus1", glm::vec3(0, 0, 1), orange);
    AddMeshToList(rhombus1);

    Mesh* rhombus2 = objects2D::CreateRhombus("rhombus2", glm::vec3(0, 0, 1), blue);
    AddMeshToList(rhombus2);

    Mesh* rhombus3 = objects2D::CreateRhombus("rhombus3", glm::vec3(0, 0, 1), red);
    AddMeshToList(rhombus3);

    Mesh* rhombus4 = objects2D::CreateRhombus("rhombus4", glm::vec3(0, 0, 1), purple);
    AddMeshToList(rhombus4);

    Mesh* life1 = objects2D::CreateSquare("life1", glm::vec3(900, 600, 0), 80, glm::vec3(0.9f, 0.1f, 0), true);
    AddMeshToList(life1);

    Mesh* life2 = objects2D::CreateSquare("life2", glm::vec3(1020, 600, 0), 80, glm::vec3(0.9f, 0.1f, 0), true);
    AddMeshToList(life2);

    Mesh* life3 = objects2D::CreateSquare("life3", glm::vec3(1140, 600, 0), 80, glm::vec3(0.9f, 0.1f, 0), true);
    AddMeshToList(life3);

    Mesh* hexa1 = objects2D::CreateHexagon("hexa1", glm::vec3(0, 0, 0), orange, turqoise);
    AddMeshToList(hexa1);

    Mesh* hexa2 = objects2D::CreateHexagon("hexa2", glm::vec3(0, 0, 0), blue, turqoise);
    AddMeshToList(hexa2);

    Mesh* hexa3 = objects2D::CreateHexagon("hexa3", glm::vec3(0, 0, 0), red, turqoise);
    AddMeshToList(hexa3);

    Mesh* hexa4 = objects2D::CreateHexagon("hexa4", glm::vec3(0, 0, 0), purple, turqoise);
    AddMeshToList(hexa4);

    Mesh* star = objects2D::CreateStar("star", glm::vec3(0, 0, 3), glm::vec3(0.9f, 0.8f, 0));
    AddMeshToList(star);

    Mesh* projectileOrange = objects2D::CreateStar("projectileOrange", glm::vec3(0, 0, 3), orange);
    AddMeshToList(projectileOrange);

    Mesh* projectileBlue = objects2D::CreateStar("projectileBlue", glm::vec3(0, 0, 3), blue);
    AddMeshToList(projectileBlue);

    Mesh* projectileRed = objects2D::CreateStar("projectileRed", glm::vec3(0, 0, 3), red);
    AddMeshToList(projectileRed);

    Mesh* projectilePurple = objects2D::CreateStar("projectilePurple", glm::vec3(0, 0, 3), purple);
    AddMeshToList(projectilePurple);
}


void Tema1::FrameStart()
{
    glClearColor(0.15, 0.15, 0.15, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{   
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], glm::mat3(1));

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square4"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square5"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square6"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square7"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square8"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["square9"], shaders["VertexColor"], glm::mat3(1));
    // patrarele ce vor cuprinde romburi
    RenderMesh2D(meshes["frame1"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["frame2"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["frame3"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["frame4"], shaders["VertexColor"], glm::mat3(1));

    // plasez romburile in patrate, urmat de stele sub ele
    int xLeft = 55, yDown = 80;
    RenderMesh2D(meshes["rhombus1"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(134, 640));

    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(134 - xLeft, 640 - yDown) * transforms2D::Scale(0.5, 0.5));

    RenderMesh2D(meshes["rhombus2"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(334, 640));

    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(334 - xLeft, 640 - yDown) * transforms2D::Scale(0.5, 0.5));
    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(334 - xLeft + 45, 640 - yDown) * transforms2D::Scale(0.5, 0.5));

    RenderMesh2D(meshes["rhombus3"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(534, 640));

    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(534 - xLeft, 640 - yDown) * transforms2D::Scale(0.5, 0.5));
    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(534 - xLeft + 45, 640 - yDown) * transforms2D::Scale(0.5, 0.5));

    RenderMesh2D(meshes["rhombus4"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(734, 640));

    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(734 - xLeft, 640 - yDown) * transforms2D::Scale(0.5, 0.5));
    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(734 - xLeft + 45, 640 - yDown) * transforms2D::Scale(0.5, 0.5));
    RenderMesh2D(meshes["star"], shaders["VertexColor"], glm::mat3(1) * transforms2D::Translate(734 - xLeft + 90, 640 - yDown) * transforms2D::Scale(0.5, 0.5));

    RenderMesh2D(meshes["life1"], shaders["VertexColor"], glm::mat3(1));

    glm::mat3 lifeMat1 = glm::mat3(1);
    if (lifesRemaining == 0) {
        lifeMat1 *= transforms2D::Scale(0, 0);
    }

    RenderMesh2D(meshes["life1"], shaders["VertexColor"], lifeMat1);

    glm::mat3 lifeMat2 = glm::mat3(1);
    if (lifesRemaining <= 1)
        lifeMat2 *= transforms2D::Scale(0, 0);
    RenderMesh2D(meshes["life2"], shaders["VertexColor"], lifeMat2);


    glm::mat3 lifeMat3 = glm::mat3(1);
    if (lifesRemaining <= 2) 
        lifeMat3 *= transforms2D::Scale(0, 0);

    RenderMesh2D(meshes["life3"], shaders["VertexColor"], lifeMat3);

    // initial sunt 5 stele, aflate sub cele 3 vieti
    if (!addedInitialStars) {
        addedStarsMatrices.push_back(glm::mat3(1) * transforms2D::Translate(855, 575) * transforms2D::Scale(0.5, 0.5));
        addedStarsMatrices.push_back(glm::mat3(1) * transforms2D::Translate(855 + 45, 575) * transforms2D::Scale(0.5, 0.5));
        addedStarsMatrices.push_back(glm::mat3(1) * transforms2D::Translate(855 + 90, 575) * transforms2D::Scale(0.5, 0.5));
        addedStarsMatrices.push_back(glm::mat3(1) * transforms2D::Translate(855 + 135, 575) * transforms2D::Scale(0.5, 0.5));
        addedStarsMatrices.push_back(glm::mat3(1) * transforms2D::Translate(855 + 180, 575) * transforms2D::Scale(0.5, 0.5));
        addedInitialStars = true;
    }

    vector<int> enemyHeights = { 20, 180, 340 };
    float initialX = (float)window->GetResolution().x; // Initial x-coordinate for hexagons
    float translateX = 0;

    float current_time = (float)clock() / CLOCKS_PER_SEC; // timpul curent
    static float lastSpawnTimeHex = current_time;
    static float lastSpawnTimeStar = current_time;
    static float lastSpawnTimeProjectile = current_time;

    float timeHex = current_time - lastSpawnTimeHex;
    float timeStar = current_time - lastSpawnTimeStar;
    float timeProjectile = current_time - lastSpawnTimeProjectile;

    // generez hexagon la interval de timp aleatoriu
    float randtimeHexagon = 6.0f + rand() % 4; // intre 6 - 9 secunde
    if (timeHex > randtimeHexagon) {
        lastSpawnTimeHex = current_time;
        
        int hexagonLevel = rand() % 3;
        int hexagonType = rand() % 4 + 1;

        hexagonModelMatrices.push_back(glm::mat3(1) * transforms2D::Translate(initialX, enemyHeights[hexagonLevel] + 62));
        hexagonLevels.push_back(hexagonLevel);
        hexagonTypes.push_back(hexagonType);
        hexagonHits.push_back(0); // initial nelovit de niciun proiectil
    }
    
    // GENEREZ CATE 3 STELE LA INTERVALE ALEATORII (si in primul moment), in locuri aleatorii
    float randTimeStars = 10.0f + rand() % 5; // 10 - 14 secunde
    if (timeStar > randTimeStars || addFirstStars == false && timeStar > 1.0f) {
        lastSpawnTimeStar = current_time;
		glm::mat3 starMatrix1 = glm::mat3(1);
        glm::mat3 starMatrix2 = glm::mat3(1);
        glm::mat3 starMatrix3 = glm::mat3(1);
        starMatrix1 *= transforms2D::Translate(rand() % 1200 + 50, rand() % 650 + 50);
        starMatrix2 *= transforms2D::Translate(rand() % 1200 + 50, rand() % 650 + 50);
        starMatrix3 *= transforms2D::Translate(rand() % 1200 + 50, rand() % 650 + 50);
        starModelMatrices.push_back(starMatrix1);
        starModelMatrices.push_back(starMatrix2);
        starModelMatrices.push_back(starMatrix3);
        
        addFirstStars = true;
    }

    for (int i = 0; i < starModelMatrices.size(); i++) {
		RenderMesh2D(meshes["star"], shaders["VertexColor"], starModelMatrices[i]);
	}

    // GENEREZ PROIECTILELE
    // HEX_CULOARE[0] = primul rand (de jos), HEX_CULOARE[1] = al doilea rand, HEX_CULOARE[2] = al treilea rand

    float randTimeProj;
    randTimeProj = 3.0f; // generez proiectile la cate 3 secunde
    if (timeProjectile > randTimeProj) {
        // cazuri portocaliu, pe cele 9 patrate (daca am romb in patratul XY si sunt hexagoane de acel tip pe linie)
        if (rombOrange11 && hexOrange[2]) {
            lastSpawnTimeProjectile = current_time;
			sendProjectiles(200, 395, 1);
        }
        if (rombOrange12 && hexOrange[2]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(360, 395, 1);
		}
        if (rombOrange13 && hexOrange[2]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(520, 395, 1);
        }
        if (rombOrange21 && hexOrange[1]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(200, 235, 1);
		}
        if (rombOrange22 && hexOrange[1]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(360, 235, 1);
        }
        if (rombOrange23 && hexOrange[1]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(520, 235, 1);
        }
        if (rombOrange31 && hexOrange[0]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(200, 75, 1);
		}
        if (rombOrange32 && hexOrange[0]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(360, 75, 1);
		}
        if (rombOrange33 && hexOrange[0]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(520, 75, 1);
		}

        // cazuri albastru, pe cele 9 patrate

        if (rombBlue11 && hexBlue[2]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(200, 395, 2);
		}
        if (rombBlue12 && hexBlue[2]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(360, 395, 2);
        }
        if (rombBlue13 && hexBlue[2]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(520, 395, 2);
		}
        if (rombBlue21 && hexBlue[1]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(200, 235, 2);
		}
        if (rombBlue22 && hexBlue[1]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(360, 235, 2);
		}
        if (rombBlue23 && hexBlue[1]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(520, 235, 2);
        }
        if (rombBlue31 && hexBlue[0]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(200, 75, 2);
        }
        if (rombBlue32 && hexBlue[0]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(360, 75, 2);
        }
        if (rombBlue33 && hexBlue[0]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(520, 75, 2);
		}

        // cazuri rosu, pe cele 9 patrate
        if (rombRed11 && hexRed[2]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(200, 395, 3);
        }
        if (rombRed12 && hexRed[2]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(360, 395, 3);
		}
        if (rombRed13 && hexRed[2]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(520, 395, 3);
		}
        if (rombRed21 && hexRed[1]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(200, 235, 3);
        }
        if (rombRed22 && hexRed[1]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(360, 235, 3);
        }
        if (rombRed23 && hexRed[1]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(520, 235, 3);
		}
        if (rombRed31 && hexRed[0]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(200, 75, 3);
        }
        if (rombRed32 && hexRed[0]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(360, 75, 3);
		}
        if (rombRed33 && hexRed[0]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(520, 75, 3);
        }

        // cazuri mov, pe cele 9 patrate
        if (rombPurple11 && hexPurple[2]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(200, 395, 4);
		}
        if (rombPurple12 && hexPurple[2]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(360, 395, 4);
		}
        if (rombPurple13 && hexPurple[2]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(520, 395, 4);
        }
        if (rombPurple21 && hexPurple[1]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(200, 235, 4);
		}
        if (rombPurple22 && hexPurple[1]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(360, 235, 4);
        }
        if (rombPurple23 && hexPurple[1]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(520, 235, 4);
        }
        if (rombPurple31 && hexPurple[0]) {
            lastSpawnTimeProjectile = current_time;
            sendProjectiles(200, 75, 4);
        }
        if (rombPurple32 && hexPurple[0]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(360, 75, 4);
		}
        if (rombPurple33 && hexPurple[0]) {
			lastSpawnTimeProjectile = current_time;
			sendProjectiles(520, 75, 4);
		}
    }

    // VERIFIC COLIZIUNEA HEXAGON-PROIECTILE
    for (int i = 0; i < hexagonModelMatrices.size(); i++) {
        for (int j = 0; j < projectileMatrices.size(); j++) {
            glm::vec3 hexagonCenter = glm::vec3(hexagonModelMatrices[i][2]);
            float hexagonRadius = 45;

            glm::vec3 starCenter = glm::vec3(projectileMatrices[j][2]);
            float starRadius = 40;

            if (isCircleCollision(hexagonCenter, hexagonRadius, starCenter, starRadius) && collisionProcessed[i][j] == false && hexagonTypes[i] == projectileTypes[j] && hexagonRemoved[i] == false && projectileRemoved[j] == false) {

                scaleHexagon[i] = 1.0f;
                collisionProcessed[i][j] = true;
                hexagonHits[i]++;

                if (hexagonHits[i] == 3) {
                    shouldScale[i] = true;
                    // decrementez numarul de hexagoane de tipul respectiv de pe linie
                    switch (hexagonTypes[i]) {
                        case 1:
                            hexOrange[hexagonLevels[i]]--;
						    break;
                        case 2:
						    hexBlue[hexagonLevels[i]]--;
                            break;
                        case 3:
                            hexRed[hexagonLevels[i]]--;
							break;
                        default:
							hexPurple[hexagonLevels[i]]--;
                    }
                }
                // dispare proiectilul dupa coliziune
                projectileMatrices[j] *= transforms2D::Scale(0, 0);
                projectileMatrices[j] *= transforms2D::Translate(10000, 10000);
                projectileRemoved[j] = true;
            }

            // animatia de disparitie a hexagonului
            if (isCircleCollision(hexagonCenter, hexagonRadius, starCenter, starRadius) && hexagonHits[i] >= 3 && scaleHexagon[i] > 0.1f && hexagonTypes[i] == projectileTypes[j]) {
                if (shouldScale[i]) {
                    scaleHexagon[i] -= 2 * deltaTimeSeconds;
                    scaleHexagon[i] = max(scaleHexagon[i], 0.0f);

                    hexagonModelMatrices[i] *= transforms2D::Scale(scaleHexagon[i], scaleHexagon[i]);
                    hexagonRemoved[i] = true;
                }
            }
        }
    }

    // VERIFIC COLIZIUNEA ROMB - HEXAGON
    for (int i = 0; i < hexagonModelMatrices.size(); i++) {
        for (int j = 0; j < rhombusMatrices.size(); j++) {
            glm::vec3 hexagonCenter = glm::vec3(hexagonModelMatrices[i][2]);
            float hexagonRadius = 45;

            glm::vec3 rhombusCenter = glm::vec3(rhombusMatrices[j][2]);
            float rhombusRadius = 40;

            if(isCircleCollision(hexagonCenter, hexagonRadius, rhombusCenter, rhombusRadius) && !collisionProcessedRhombus[i][j] && !rhombusRemoved[j] && !hexagonRemoved[i]) {
                collisionProcessedRhombus[i][j] = true;

                // resetez variabilele pentru patratul respectiv
                if (rhombusSquare[j] == 1) {
                    toDrag31 = false;
                    rombRed31 = false;
                    rombBlue31 = false;
                    rombOrange31 = false;
                    rombPurple31 = false;
                }
                else if (rhombusSquare[j] == 2) {
                    toDrag32 = false;
                    rombRed32 = false;
                    rombBlue32 = false;
                    rombOrange32 = false;
                    rombPurple32 = false;
                }
                else if (rhombusSquare[j] == 3) {
                    toDrag33 = false;
                    rombRed33 = false;
                    rombBlue33 = false;
                    rombOrange33 = false;
                    rombPurple33 = false;
                }
                else if (rhombusSquare[j] == 4) {
                    toDrag21 = false;
                    rombRed21 = false;
                    rombBlue21 = false;
                    rombOrange21 = false;
                    rombPurple21 = false;
                }
                else if (rhombusSquare[j] == 5) {
                    toDrag22 = false;
                    rombRed22 = false;
                    rombBlue22 = false;
                    rombOrange22 = false;
                    rombPurple22 = false;
                }
                else if (rhombusSquare[j] == 6) {
                    toDrag23 = false;
                    rombRed23 = false;
                    rombBlue23 = false;
                    rombOrange23 = false;
                    rombPurple23 = false;
                }
                else if (rhombusSquare[j] == 7) {
                    toDrag11 = false;
                    rombRed11 = false;
                    rombBlue11 = false;
                    rombOrange11 = false;
                    rombPurple11 = false;
                }
                else if (rhombusSquare[j] == 8) {
                    toDrag12 = false;
                    rombRed12 = false;
                    rombBlue12 = false;
                    rombOrange12 = false;
                    rombPurple12 = false;
                }
                else if (rhombusSquare[j] == 9) {
                    toDrag13 = false;
                    rombRed13 = false;
                    rombBlue13 = false;
                    rombOrange13 = false;
                    rombPurple13 = false;
                }

                rhombusRemoved[j] = true;
            }
            // ANIMATIE DISPARITIE ROMB
            if (isCircleCollision(hexagonCenter, hexagonRadius, rhombusCenter, rhombusRadius)) {
                if (rhombusRemoved[j]) {
                    scaleRhombus[j] -= 2 * deltaTimeSeconds;
                    scaleRhombus[j] = max(scaleRhombus[j], 0.0f);

                    rhombusMatrices[j] *= transforms2D::Scale(scaleRhombus[j], scaleRhombus[j]);
                }
            }
            
        }
    }
    for (int j = 0; j < rhombusMatrices.size(); j++) {
        // animatie disparitie romb cand ii dau click dreapta
        if (rhombusSquare[j] == squareToRemove) {
            scaleRhombus[j] -= 2 * deltaTimeSeconds;
            scaleRhombus[j] = max(scaleRhombus[j], 0.0f);     
            rhombusMatrices[j] *= transforms2D::Scale(scaleRhombus[j], scaleRhombus[j]);

            if (rhombusSquare[j] == 1) {
                toDrag31 = false;
                rombRed31 = false;
                rombBlue31 = false;
                rombOrange31 = false;
                rombPurple31 = false;
            }
            else if (rhombusSquare[j] == 2) {
                toDrag32 = false;
                rombRed32 = false;
                rombBlue32 = false;
                rombOrange32 = false;
                rombPurple32 = false;
            }
            else if (rhombusSquare[j] == 3) {
                toDrag33 = false;
                rombRed33 = false;
                rombBlue33 = false;
                rombOrange33 = false;
                rombPurple33 = false;
            }
            else if (rhombusSquare[j] == 4) {
                toDrag21 = false;
                rombRed21 = false;
                rombBlue21 = false;
                rombOrange21 = false;
                rombPurple21 = false;
            }
            else if (rhombusSquare[j] == 5) {
                toDrag22 = false;
                rombRed22 = false;
                rombBlue22 = false;
                rombOrange22 = false;
                rombPurple22 = false;
            }
            else if (rhombusSquare[j] == 6) {
                toDrag23 = false;
                rombRed23 = false;
                rombBlue23 = false;
                rombOrange23 = false;
                rombPurple23 = false;
            }
            else if (rhombusSquare[j] == 7) {
                toDrag11 = false;
                rombRed11 = false;
                rombBlue11 = false;
                rombOrange11 = false;
                rombPurple11 = false;
            }
            else if (rhombusSquare[j] == 8) {
                toDrag12 = false;
                rombRed12 = false;
                rombBlue12 = false;
                rombOrange12 = false;
                rombPurple12 = false;
            }
            else if (rhombusSquare[j] == 9) {
                toDrag13 = false;
                rombRed13 = false;
                rombBlue13 = false;
                rombOrange13 = false;
                rombPurple13 = false;
            }
            rhombusRemoved[j] = true;
        }
    }

    // RENDER STELELE APARUTE RANDOM
    for (int i = 0; i < addedStarsMatrices.size() && i < 10; i++) {
        RenderMesh2D(meshes["star"], shaders["VertexColor"], addedStarsMatrices[i]);
    }

    // GENEREZ HEXAGOANE 
    for (int i = 0; i < hexagonModelMatrices.size(); i++) {
        translateX = 80 * deltaTimeSeconds;
        hexagonModelMatrices[i] *= transforms2D::Translate(-translateX, 0);

        // random una din cele 4 variante
        switch (hexagonTypes[i]) {
            // dupa cazuri, dau render la hexagonul respectiv si incrementez nr de hexagone de tipul respectiv
            case 1:
                RenderMesh2D(meshes["hexa1"], shaders["VertexColor"], hexagonModelMatrices[i]);
                if (toCountHexagon[i] == false) {
                    hexOrange[hexagonLevels[i]]++;
                    toCountHexagon[i] = true;
                }
                break;
            case 2:
                RenderMesh2D(meshes["hexa2"], shaders["VertexColor"], hexagonModelMatrices[i]);
                if (toCountHexagon[i] == false) {
                    hexBlue[hexagonLevels[i]]++;
                    toCountHexagon[i] = true;
                }
                break;
            case 3:
				RenderMesh2D(meshes["hexa3"], shaders["VertexColor"], hexagonModelMatrices[i]);
                if (toCountHexagon[i] == false) {
                    hexRed[hexagonLevels[i]]++;
                    toCountHexagon[i] = true;
                }
				break;
            default:
                RenderMesh2D(meshes["hexa4"], shaders["VertexColor"], hexagonModelMatrices[i]);
                if (toCountHexagon[i] == false) {
                    hexPurple[hexagonLevels[i]]++;
                    toCountHexagon[i] = true;
                }
        }

        // HEXAGONUL IESE DIN ECRAN, MINUS O VIATA
        if (hexagonModelMatrices[i][2][0] <= 50) {
            if (hexagonRemoved[i] == false) {
                hexagonRemoved[i] = true;
                // oricare ar fi cazul, o viata este scazuta
                switch (hexagonTypes[i]) {
					case 1:
						hexOrange[hexagonLevels[i]]--;
                        lifesRemaining--;
						break;
					case 2:
						hexBlue[hexagonLevels[i]]--;
                        lifesRemaining--;
						break;
					case 3:
						hexRed[hexagonLevels[i]]--;
                        lifesRemaining--;
						break;
					default:
						hexPurple[hexagonLevels[i]]--;
                        lifesRemaining--;
				}
                hexagonModelMatrices[i] *= transforms2D::Scale(0, 0); 
            }
            if (lifesRemaining == 0) {
				cout << "GAME OVER" << endl;
				exit(0);
			}
		}
    }
    
    // RENDER la proiectile
    for (int j = 0; j < projectileMatrices.size(); j++) {
        // centrul proiectilului
        float x = projectileMatrices[j][2][0];
        float y = projectileMatrices[j][2][1];

        float rotationSpeed = -45.0f;
        float rotationAngle = rotationSpeed * 0.15 * deltaTimeSeconds;
        glm::mat3 rotation = transforms2D::Rotate(rotationAngle);
        glm::mat3 centerRotation = transforms2D::Translate(x, y) * rotation * transforms2D::Translate(-x, -y);

        float translateProjectile = 250 * deltaTimeSeconds;
        projectileMatrices[j] = transforms2D::Translate(translateProjectile, 0) * centerRotation * projectileMatrices[j];
        
        string projectileName;
        switch (projectileTypes[j]) {
			case 1:
				projectileName = "projectileOrange";
				break;
			case 2:
				projectileName = "projectileBlue";
				break;
			case 3:
				projectileName = "projectileRed";
				break;
			default:
				projectileName = "projectilePurple";
		}
        RenderMesh2D(meshes[projectileName], shaders["VertexColor"], projectileMatrices[j]);
    }

    // Partea de DRAG, pe 4 cazuri (pentru cele 4 tipuri de romburi)
    if (mousePressed1 || mousePressed2 || mousePressed3 || mousePressed4) {
        
        // get the position of mouse x and y
        float cursorX = (float)window->GetCursorPosition().x;
        float cursorY = 720 - (float)window->GetCursorPosition().y;
        
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transforms2D::Translate(cursorX, cursorY);

        if (mousePressed1) {
			RenderMesh2D(meshes["rhombus1"], shaders["VertexColor"], modelMatrix);
        }
        else if (mousePressed2) {
            RenderMesh2D(meshes["rhombus2"], shaders["VertexColor"], modelMatrix);
        }
        else if (mousePressed3) {
            RenderMesh2D(meshes["rhombus3"], shaders["VertexColor"], modelMatrix);
		}
        else if (mousePressed4) {
            RenderMesh2D(meshes["rhombus4"], shaders["VertexColor"], modelMatrix);
		}
    }

    // Partea de DROP pe cele 9 cazuri (9 patrare), cate 4 variante fiecare (tip romb)
    if (toDrag31 == true) {
        if (color31 == 1)
            rombOrange31 = true;
        else if (color31 == 2)
            rombBlue31 = true;
        else if (color31 == 3)
            rombRed31 = true;
        else if (color31 == 4)
            rombPurple31 = true;
    }

    if (toDrag32 == true) {
        if(color32 == 1)
			rombOrange32 = true;
		else if (color32 == 2)
			rombBlue32 = true;
		else if (color32 == 3)
			rombRed32 = true;
		else if (color32 == 4)
			rombPurple32 = true;
    }

    if (toDrag33 == true) {
        if (color33 == 1)
            rombOrange33 = true;
        else if (color33 == 2)
            rombBlue33 = true;
        else if (color33 == 3)
            rombRed33 = true;
        else if (color33 == 4)
            rombPurple33 = true;
    }

    if (toDrag21 == true) {
        if (color21 == 1)
            rombOrange21 = true;
        else if (color21 == 2)
            rombBlue21 = true;
        else if (color21 == 3)
            rombRed21 = true;
        else if (color21 == 4)
            rombPurple21 = true;
    }

    if (toDrag22 == true) {
        if (color22 == 1)
            rombOrange22 = true;
        else if (color22 == 2)
            rombBlue22 = true;
        else if (color22 == 3)
            rombRed22 = true;
        else if (color22 == 4)
            rombPurple22 = true;
    }

    if (toDrag23 == true) {
        if (color23 == 1)
            rombOrange23 = true;
        else if (color23 == 2)
            rombBlue23 = true;
        else if (color23 == 3)
            rombRed23 = true;
        else if (color23 == 4)
            rombPurple23 = true;
    }

    if (toDrag11 == true) {
        if (color11 == 1)
            rombOrange11 = true;
        else if (color11 == 2)
            rombBlue11 = true;
        else if (color11 == 3)
            rombRed11 = true;
        else if (color11 == 4)
            rombPurple11 = true;
    }

    if (toDrag12 == true) {
        if (color12 == 1)
            rombOrange12 = true;
        else if (color12 == 2)
            rombBlue12 = true;
        else if (color12 == 3)
            rombRed12 = true;
        else if (color12 == 4)
            rombPurple12 = true;
    }
    
    if (toDrag13 == true) {
        if (color13 == 1)
            rombOrange13 = true;
        else if (color13 == 2)
            rombBlue13 = true;
        else if (color13 == 3)
            rombRed13 = true;
        else if (color13 == 4)
            rombPurple13 = true;
    }

    for (int i = 0; i < rhombusMatrices.size(); i++) {
        string rhombusName = "rhombus" + to_string(rhombusTypes[i]);
        RenderMesh2D(meshes[rhombusName], shaders["VertexColor"], rhombusMatrices[i]);
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    float cursorX = (float)mouseX;
    float cursorY = 720 - (float)mouseY;
    squareToRemove = 0;

    // mouse apasat pe primul tip de romb. Costul = 1 stea
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(60, 580, 0), 120) && button == 1) {
        if (starCount >= 1) {
            mousePressed1 = true;
            tipRombDrag = 1;
        }
	}
    // romb 2, cost 2 stele
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(260, 580, 0), 120) && button == 1) {
        if (starCount >= 2) {
			mousePressed2 = true;
			tipRombDrag = 2;
		}
    }
    // romb 3, cost 2 stele
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(460, 580, 0), 120) && button == 1) {
        if (starCount >= 2) {
            mousePressed3 = true;
            tipRombDrag = 3;
        }
    }
    // romb 4, cost 3 stele
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(660, 580, 0), 120) && button == 1) {
        if (starCount >= 3) {
            mousePressed4 = true;
            tipRombDrag = 4;
        }
    }

    // CLICK DREAPTA PE ROMBURI DIN CELE 9 PATRARE

    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(100, 20, 0), 120) && button == 2) {
        squareToRemove = 1;
    }
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(260, 20, 0), 120) && button == 2) {
        squareToRemove = 2;
	}
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(420, 20, 0), 120) && button == 2) {
        squareToRemove = 3;
    }
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(100, 180, 0), 120) && button == 2) {
        squareToRemove = 4;
	}
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(260, 180, 0), 120) && button == 2) {
        squareToRemove = 5;
    }
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(420, 180, 0), 120) && button == 2) {
        squareToRemove = 6;
    }
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(100, 340, 0), 120) && button == 2) {
        squareToRemove = 7;
	}
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(260, 340, 0), 120) && button == 2) {
        squareToRemove = 8;
	}
    if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(420, 340, 0), 120) && button == 2) {
        squareToRemove = 9;
    }



    for (int i = 0; i < starModelMatrices.size(); i++) {
        if (isPointInsideStar(glm::vec3(cursorX, cursorY, 0), glm::vec3(starModelMatrices[i][2]), 75) && button == 1) {
			starModelMatrices.erase(starModelMatrices.begin() + i); // sterg steaua
            addedStarsMatrices.push_back(glm::mat3(1) * transforms2D::Translate(855 + 45 * starCount, 575) * transforms2D::Scale(0.5, 0.5));
            starCount++;
		}
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    float cursorX = (float)mouseX;
    float cursorY = 720 - (float)mouseY;

    bool draggedSuccessfully = false;

    if (mousePressed1 || mousePressed2 || mousePressed3 || mousePressed4) {
        // cursorul e eliberat in patratul 1 si nu exista alt romb acolo
        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(100, 20, 0), 120) && !toDrag31) {
            rhombusSquare.push_back(1);
            dragRhombusX1 = 100 + 60;
            dragRhombusY1 = 20 + 60;
            toDrag31 = true;

            // ii adaug matricea in vector
            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(100 + 60, 20 + 60));

            if(mousePressed1)
				color31 = 1;
			else if (mousePressed2)
                color31 = 2;
			else if (mousePressed3)
                color31 = 3;
			else if (mousePressed4)
                color31 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(260, 20, 0), 120) && !toDrag32) {
            rhombusSquare.push_back(2);
            dragRhombusX2 = 260 + 60;
            dragRhombusY2 = 20 + 60;
            toDrag32 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(260 + 60, 20 + 60));

            if (mousePressed1)
                color32 = 1;
            else if (mousePressed2)
                color32 = 2;
            else if (mousePressed3)
                color32 = 3;
            else if (mousePressed4)
                color32 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(420, 20, 0), 120) && !toDrag33) {
            rhombusSquare.push_back(3);
            dragRhombusX3 = 420 + 60;
            dragRhombusY3 = 20 + 60;
            toDrag33 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(420 + 60, 20 + 60));

            if (mousePressed1)
                color33 = 1;
            else if (mousePressed2)
                color33 = 2;
            else if (mousePressed3)
                color33 = 3;
            else if (mousePressed4)
                color33 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(100, 180, 0), 120) && !toDrag21) {
            rhombusSquare.push_back(4);
            dragRhombusX4 = 100 + 60;
            dragRhombusY4 = 180 + 60;
            toDrag21 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(100 + 60, 180 + 60));

            if (mousePressed1)
                color21 = 1;
            else if (mousePressed2)
                color21 = 2;
            else if (mousePressed3)
                color21 = 3;
            else if (mousePressed4)
                color21 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(260, 180, 0), 120) && !toDrag22) {
            rhombusSquare.push_back(5);
            dragRhombusX5 = 260 + 60;
            dragRhombusY5 = 180 + 60;
            toDrag22 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(260 + 60, 180 + 60));

            if (mousePressed1)
                color22 = 1;
            else if (mousePressed2)
                color22 = 2;
            else if (mousePressed3)
                color22 = 3;
            else if (mousePressed4)
                color22 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(420, 180, 0), 120) && !toDrag23) {
            rhombusSquare.push_back(6);
            dragRhombusX6 = 420 + 60;
            dragRhombusY6 = 180 + 60;
            toDrag23 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(420 + 60, 180 + 60));

            if (mousePressed1)
                color23 = 1;
            else if (mousePressed2)
                color23 = 2;
            else if (mousePressed3)
                color23 = 3;
            else if (mousePressed4)
                color23 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(100, 340, 0), 120) && !toDrag11) {
            rhombusSquare.push_back(7);
            dragRhombusX7 = 100 + 60;
            dragRhombusY7 = 340 + 60;
            toDrag11 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(100 + 60, 340 + 60));

            if (mousePressed1)
                color11 = 1;
            else if (mousePressed2)
                color11 = 2;
            else if (mousePressed3)
                color11 = 3;
            else if (mousePressed4)
                color11 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(260, 340, 0), 120) && !toDrag12) {
            rhombusSquare.push_back(8);
            dragRhombusX8 = 260 + 60;
            dragRhombusY8 = 340 + 60;
            toDrag12 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(260 + 60, 340 + 60));

            if (mousePressed1)
                color12 = 1;
            else if (mousePressed2)
                color12 = 2;
            else if (mousePressed3)
                color12 = 3;
            else if (mousePressed4)
                color12 = 4;

            draggedSuccessfully = true;
        }

        if (isPointInsideSquare(glm::vec3(cursorX, cursorY, 0), glm::vec3(420, 340, 0), 120) && !toDrag13) {
            rhombusSquare.push_back(9);
            dragRhombusX9 = 420 + 60;
            dragRhombusY9 = 340 + 60;
            toDrag13 = true;

            rhombusMatrices.push_back(glm::mat3(1) * transforms2D::Translate(420 + 60, 340 + 60));

            if (mousePressed1)
                color13 = 1;
            else if (mousePressed2)
                color13 = 2;
            else if (mousePressed3)
                color13 = 3;
            else if (mousePressed4)
                color13 = 4;

            draggedSuccessfully = true;
        }

        if (draggedSuccessfully) {
            if (mousePressed1) {
                rhombusTypes.push_back(1);
                starCount -= 1;
                addedStarsMatrices.erase(addedStarsMatrices.begin() + starCount);
            }
            else if (mousePressed2 || mousePressed3) {
                if (mousePressed2) {
					rhombusTypes.push_back(2);
				}
				else {
                    rhombusTypes.push_back(3);
                }

                for (int i = 0; i < 2; i++) {
                    starCount -= 1;
					addedStarsMatrices.erase(addedStarsMatrices.begin() + starCount);
                }
            }
            else if (mousePressed4) {
                rhombusTypes.push_back(4);
                for (int i = 0; i < 3; i++) {
					starCount -= 1;
                    addedStarsMatrices.erase(addedStarsMatrices.begin() + starCount);
                }
            }
        }
    }
    mousePressed1 = mousePressed2 = mousePressed3 = mousePressed4 = false;
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
