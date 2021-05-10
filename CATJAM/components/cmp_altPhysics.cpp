#include "cmp_altPhysics.h"
#include "altSystemPhysics.h"

using namespace std;
using namespace sf;

using namespace AltPhysics;

void AltPhysicsComponent::update(double dt) {
    _parent->setPosition(invert_height(bv2_to_sv2(_body->GetPosition())));
    _parent->setRotation((180 / b2_pi) * _body->GetAngle());
}

AltPhysicsComponent::AltPhysicsComponent(Entity* p, bool dyn,
    const Vector2f& size)
    : Component(p), _dynamic(dyn) {

    b2BodyDef BodyDef;
    // Is Dynamic(moving), or static(Stationary)
    BodyDef.type = _dynamic ? b2_dynamicBody : b2_staticBody;
    BodyDef.position = sv2_to_bv2(invert_height(p->getPosition()));

    // Create the body
    _body = AltPhysics::GetWorld()->CreateBody(&BodyDef);
    _body->SetActive(true);
    {
        // Create the fixture shape
        b2PolygonShape Shape;
        // SetAsBox box takes HALF-Widths!
        Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
        b2FixtureDef FixtureDef;
        // Fixture properties
        // FixtureDef.density = _dynamic ? 10.f : 0.f;
        FixtureDef.friction = _dynamic ? 0.1f : 0.8f;
        FixtureDef.restitution = .2;
        FixtureDef.shape = &Shape;
        // Add to body
        _fixture = _body->CreateFixture(&FixtureDef);
        //_fixture->SetRestitution(.9)
        FixtureDef.restitution = .2;
    }

    // An ideal Pod/capusle shape should be used for hte player,
    // this isn't built into B2d, but we can combine two shapes to do so.
    // This would allwo the player to go up steps
    /*
      BodyDef.bullet = true;
      b2PolygonShape shape1;
      shape1.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
      {
        b2PolygonShape poly ;
        poly.SetAsBox(0.45f, 1.4f);
        b2FixtureDef FixtureDefPoly;

        FixtureDefPoly.shape = &poly;
        _body->CreateFixture(&FixtureDefPoly);

      }
      {
        b2CircleShape circle;
        circle.m_radius = 0.45f;
        circle.m_p.Set(0, -1.4f);
        b2FixtureDef FixtureDefCircle;
        FixtureDefCircle.shape = &circle;
        _body->CreateFixture(&FixtureDefCircle);
      }
    */
}

void AltPhysicsComponent::setFriction(float r) { _fixture->SetFriction(r); }

void AltPhysicsComponent::setMass(float m) { _fixture->SetDensity(m); }

void AltPhysicsComponent::teleport(const sf::Vector2f& v) {
    _body->SetTransform(sv2_to_bv2(invert_height(v)), 0.0f);
}

const sf::Vector2f AltPhysicsComponent::getVelocity() const {
    return bv2_to_sv2(_body->GetLinearVelocity(), true);
}
void AltPhysicsComponent::setVelocity(const sf::Vector2f& v) {
    _body->SetLinearVelocity(sv2_to_bv2(v, true));
}

b2Fixture* const AltPhysicsComponent::getFixture() const { return _fixture; }

AltPhysicsComponent::~AltPhysicsComponent() {
    auto a = AltPhysics::GetWorld();
    _body->SetActive(false);
    AltPhysics::GetWorld()->DestroyBody(_body);
    // delete _body;
    _body = nullptr;
}

void AltPhysicsComponent::render() {}

void AltPhysicsComponent::impulse(const sf::Vector2f& i) {
    auto a = b2Vec2(i.x, i.y * -1.0f);
    _body->ApplyLinearImpulseToCenter(a, true);
}

void AltPhysicsComponent::dampen(const sf::Vector2f& i) {
    auto vel = _body->GetLinearVelocity();
    vel.x *= i.x;
    vel.y *= i.y;
    _body->SetLinearVelocity(vel);
}

bool AltPhysicsComponent::isTouching(const AltPhysicsComponent& pc) const {
    b2Contact* bc;
    return isTouching(pc, bc);
}

bool AltPhysicsComponent::isTouching(const AltPhysicsComponent& pc,
    b2Contact const* bc) const {
    const auto _otherFixture = pc.getFixture();
    const auto& w = *AltPhysics::GetWorld();
    const auto contactList = w.GetContactList();
    const auto clc = w.GetContactCount();
    for (int32 i = 0; i < clc; i++) {
        const auto& contact = (contactList[i]);
        if (contact.IsTouching() && ((contact.GetFixtureA() == _fixture &&
            contact.GetFixtureA() == _otherFixture) ||
            (contact.GetFixtureA() == _otherFixture &&
                contact.GetFixtureA() == _fixture))) {
            bc = &contact;
            return true;
        }
    }
    return false;
}

std::vector<const b2Contact const*> AltPhysicsComponent::getTouching() const {
    std::vector<const b2Contact const*> ret;

    b2ContactEdge* edge = _body->GetContactList();
    while (edge != NULL) {
        const b2Contact* contact = edge->contact;
        if (contact->IsTouching()) {
            ret.push_back(contact);
        }
        edge = edge->next;
    }

    return ret;
}

void AltPhysicsComponent::setRestitution(float r) {
    _fixture->SetRestitution(r);
}
