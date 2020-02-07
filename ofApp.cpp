#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(0.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	this->size = 10;
	for (auto x = -450; x <= 450; x += this->size) {

		for (auto y = -450; y <= 450; y += this->size) {

			this->box_info_list.push_back(make_pair(glm::vec2(x, y), 0.f));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	auto max_height = 150;
	auto threshold = 150;
	auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
	auto noise_location = glm::vec2(ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.01), 0, 1, -300, 300), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.01), 0, 1, -300, 300));
	this->log_list.push_back(glm::vec3(noise_location, 50));
	while (this->log_list.size() > 30) {
	
		this->log_list.erase(this->log_list.begin());
	}
	
	for (auto& box_info : this->box_info_list) {

		auto distance = glm::distance(noise_location, glm::vec2(box_info.first.x, box_info.first.y));
		if (distance > threshold - 20 && distance < threshold + 20) {

			box_info.second = box_info.second >= 1 ? 1 : box_info.second + ofMap(abs(distance - threshold), 0, 20, 0.125, 0);
		}
		else {

			box_info.second = box_info.second <= 0 ? 0 : box_info.second - 0.02;
		}
	}

	for (auto& box_info : this->box_info_list) {
		
		if (box_info.second > 0) {

			auto len = ofMap(ofNoise(box_info.first.x * 0.02, box_info.first.y * 0.02, ofGetFrameNum() * 0.01), 0, 1, 0, max_height * box_info.second);
			this->setBoxToMesh(this->face, this->frame, glm::vec3(box_info.first.x, box_info.first.y, len * 0.5), this->size, this->size, len);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(295);

	ofSetColor(239, 39, 39);
	ofNoFill();
	ofSetLineWidth(1.5);

	ofBeginShape();
	ofVertices(this->log_list);
	ofEndShape();
	
	ofFill();
	ofDrawSphere(this->log_list.back(), 10);

	ofSetColor(39);
	this->face.drawFaces();

	ofSetColor(239);
	ofSetLineWidth(0.5);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size) {

	this->setBoxToMesh(face_target, frame_target, location, size, size, size);
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth) {

	int index = face_target.getVertices().size();

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * 0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * 0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * 0.5 * 0.999, depth * 0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * 0.5 * 0.999, depth * 0.5 * 0.999));

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * -0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * -0.5 * 0.999, depth * -0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.999, height * -0.5 * 0.999, depth * 0.5 * 0.999));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.999, height * -0.5 * 0.999, depth * 0.5 * 0.999));

	face_target.addIndex(index + 0); face_target.addIndex(index + 1); face_target.addIndex(index + 2);
	face_target.addIndex(index + 0); face_target.addIndex(index + 2); face_target.addIndex(index + 3);

	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 4); face_target.addIndex(index + 6); face_target.addIndex(index + 7);

	face_target.addIndex(index + 0); face_target.addIndex(index + 4); face_target.addIndex(index + 1);
	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 1);

	face_target.addIndex(index + 1); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 6); face_target.addIndex(index + 2); face_target.addIndex(index + 1);

	face_target.addIndex(index + 2); face_target.addIndex(index + 6); face_target.addIndex(index + 7);
	face_target.addIndex(index + 7); face_target.addIndex(index + 3); face_target.addIndex(index + 2);

	face_target.addIndex(index + 3); face_target.addIndex(index + 7); face_target.addIndex(index + 4);
	face_target.addIndex(index + 4); face_target.addIndex(index + 0); face_target.addIndex(index + 3);

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 1);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 0);

	frame_target.addIndex(index + 4); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 5); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 7);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 7);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}