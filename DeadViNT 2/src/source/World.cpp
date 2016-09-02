#include "../headers/World.h"

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds) : 
mTarget(outputTarget),
mFonts(fonts),
mSounds(sounds),
mWorldView(outputTarget.getDefaultView()),
mWorldBounds(0.f, 0.f, 1024, 720),
mSpawnPosition(mWorldView.getSize().x/2.f, mWorldBounds.height - mWorldView.getSize().y /2.f),
mScrollSpeed(-50.f),
mPlayerHuman(nullptr),
gameOver(false),
mCollisionCell(),
mCollisionGrid(1024,720,100,100)
{
	mSceneTexture.create(outputTarget.getSize().x, outputTarget.getSize().y);

	loadTextures();
	buildScene();

	//testCollisions();
	testSolids();
	mWorldView.setCenter(mSpawnPosition);

}

void World::loadTextures(){

	mTextures.load(Resources::Textures::Grass, "Resources/img/Grass.jpg");
	mTextures.load(Resources::Textures::Particle, "Resources/img/Particle.png");
	mTextures.load(Resources::Textures::Explosion, "Resources/img/Explosion.png");
	mTextures.load(Resources::Textures::Human, "Resources/img/Human.png");
	mTextures.load(Resources::Textures::Weapons, "Resources/img/Weapons.png");
	mTextures.load(Resources::Textures::Projectiles, "Resources/img/Bullet.png");
	mTextures.load(Resources::Textures::Solid, "Resources/img/solid/wall.png");

}

void World::buildScene(){

	//Initialize each layer
	for (std::size_t i = 0; i < LayerCount; ++i){

		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}


	//Initialize desert sprite node
	sf::Texture& texture = mTextures.get(Resources::Textures::Grass);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<Human> human(new Human(mTextures));
	human->setPosition(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f);
	mPlayerHuman = human.get();
	mSceneLayers[UpperAir]->attachChild(std::move(human));

	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

}

void World::draw(){

	if (PostEffect::isSupported()){

		mSceneTexture.clear();
		mSceneTexture.setView(mWorldView);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);

		drawGrid(100,100);

	}

	else{
		mTarget.setView(mWorldView);
		mTarget.draw(mSceneGraph);
	}
}

void World::update(sf::Time dt){
	//printf("%i\n", mSceneGraph.getNodes().size());
	
	//forward commands to the scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	
	mSceneGraph.removeWrecks();
	//mSceneGraph.clearNodes();

	mSceneGraph.update(dt, mCommandQueue);

	mPlayerHuman->setVelocity(0.f, 0.f);
	updateSounds();

	handleCollisions();
}

CommandQueue& World::getCommandQueue(){
	return mCommandQueue;
}

sf::FloatRect World::getViewBounds() const {
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}





void World::handleCollisions(){
	std::set < SceneNode::Pair > collisionPairs;
	//mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
	mSceneGraph.addNodes();
	std::vector<SceneNode*> nodes = mSceneGraph.getNodes();
	//printf("%i\n", nodes.size());
	mCollisionGrid.build(nodes);
	
	mCollisionGrid.checkCollisions(collisionPairs);
	FOREACH(SceneNode::Pair pair, collisionPairs){
		if (matchesCategories(pair, Category::Projectile, Category::Solid)){
			//printf("Solid\n");
			auto projectile = static_cast<Projectile*>(pair.first);
			projectile->destroy();
		}

		else if (matchesCategories(pair, Category::PlayerHuman, Category::Solid))
			adjustPlayerObstacle(pair.second);
		
	}

	mSceneGraph.clearNodes();
	//printf("%i\n", collisionPairs.size());
	
}

bool World::gameStatus() const{
	return gameOver;
}

void World::updateSounds(){
	mSounds.setListenerPosition(mPlayerHuman->getWorldPosition());
	mSounds.removeStoppedSounds();
}


//Much faster in Release mode
void World::testCollisions(){
	sf::Texture& texture = mTextures.get(Resources::Textures::Projectiles);
	sf::IntRect textureRect(0, 0, 20, 20);
	texture.setRepeated(true);

	for (float j = 0; j < 720.f; j+=50.f)
		for (float i = 0.f; i < 1024.f; i += 100.f){
			std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
			backgroundSprite->setPosition(i, j);
			mSceneLayers[UpperAir]->attachChild(std::move(backgroundSprite));
		}
	
}

void World::drawGrid(int width, int height){
	
	for (int i = 0; i < 720; i += height){
		sf::RectangleShape line(sf::Vector2f(1024, 2));
		line.setPosition(0,i);
		mTarget.draw(line);
	}
		
	for (int i = 0; i < 1024; i += width){
		sf::RectangleShape line(sf::Vector2f(720, 2));
		line.rotate(90);
		line.setPosition(i, 0);
		mTarget.draw(line);
	}

}

void World::testSolids(){
	sf::Texture& texture = mTextures.get(Resources::Textures::Solid);
	sf::IntRect textureRect(0, 0, 50, 50);

	for (int i = 0; i < 10; i++){
		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(200 + 50 * i, 100);
		sprite->setSolid(true);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));
	}

	for (int i = 0; i < 10; i++){
		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(200 + 50*i, 500);
		sprite->setSolid(true);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));
	}

	for (int i = 1; i < 9; i++)
		if (i != 5 && i != 4){
		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(200, 100 + 50*i);
		sprite->setSolid(true);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));
	}

	for (int i = 0; i < 9; i++){
		std::unique_ptr<SpriteNode> sprite(new SpriteNode(texture, textureRect));
		sprite->setPosition(700, 100 + 50 * i);
		sprite->setSolid(true);
		mSceneLayers[UpperAir]->attachChild(std::move(sprite));
	}
}

void World::adjustPlayerObstacle(SceneNode* obstacle){
	sf::Vector2f playerTL = sf::Vector2f(mPlayerHuman->getBoundingRect().left, mPlayerHuman->getBoundingRect().top);
	sf::Vector2f playerDR = playerTL + sf::Vector2f(mPlayerHuman->getBoundingRect().width, mPlayerHuman->getBoundingRect().height);

	sf::Vector2f obstacleTL = sf::Vector2f(obstacle->getBoundingRect().left, obstacle->getBoundingRect().top);
	sf::Vector2f obstacleDR = obstacleTL + sf::Vector2f(obstacle->getBoundingRect().width, obstacle->getBoundingRect().height);

	//printf("%f,%f\n", playerTL.y, obstacleDL.y);
	float playerLeft = playerTL.x;
	float playerRight = playerDR.x;
	float playerTop = playerTL.y;
	float playerBottom = playerDR.y;

	float obstacleLeft = obstacleTL.x;
	float obstacleRight = obstacleDR.x;
	float obstacleTop = obstacleTL.y;
	float obstacleBottom = obstacleDR.y;

	sf::FloatRect inter = unionRect(mPlayerHuman->getBoundingRect(), obstacle->getBoundingRect());
	//printf("%f,%f\n", inter.width, inter.height);
	
	if (inter.width > inter.height){
		if (playerTop < obstacleBottom && playerTop > obstacleTop)
			mPlayerHuman->setPosition(mPlayerHuman->getPosition().x, obstacleBottom + mPlayerHuman->getBoundingRect().height / 2.f);

		else if (playerBottom > obstacleTop && playerBottom < obstacleBottom)
			mPlayerHuman->setPosition(mPlayerHuman->getPosition().x, obstacleTop - mPlayerHuman->getBoundingRect().height / 2.f);
	}

	else if (inter.width < inter.height){
		if (playerRight > obstacleLeft && playerRight < obstacleRight)
			mPlayerHuman->setPosition(obstacleLeft - mPlayerHuman->getBoundingRect().width / 2.f, mPlayerHuman->getPosition().y);

		else if (playerLeft < obstacleRight && playerLeft > obstacleLeft)
			mPlayerHuman->setPosition(obstacleRight + mPlayerHuman->getBoundingRect().width / 2.f, mPlayerHuman->getPosition().y);
	}
	
}