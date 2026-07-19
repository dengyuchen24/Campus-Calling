#include "character.h"
#include "../Helper.h"

extern std::map<std::string, _Student> g_Students;
extern std::map<std::string, _Teacher> g_Teachers;
// extern std::map<std::string, _Effect> g_Effects;

DYC_BEGIN

void Character::SetExtra(ExtraKey key, std::any val)
{
	mExtra[key] = std::move(val);
}

std::wstring Character::GetName()
{
	if (Student* stu = dynamic_cast<Student*>(this))
		return dto_wstring(stu->student->name);
	else if (Teacher* tea = dynamic_cast<Teacher*>(this))
		return dto_wstring(tea->teacher->name);
	else return std::wstring();
}

int Character::GetSpeed()
{
	if (Student* stu = dynamic_cast<Student*>(this))
		return stu->student->speed
		+ static_cast<int>(stu->student->speed * mSpeedBonus);
	else if (Teacher* tea = dynamic_cast<Teacher*>(this))
		return tea->teacher->speed
		+ static_cast<int>(tea->teacher->speed * mSpeedBonus);
	else return 1;
}

void Character::SetSpeedBonus(float deltaSpeed)
{
	mSpeedBonus += deltaSpeed;
}

int Character::GetMoveVal() const
{
	return mMoveVal;
}

void Character::RecalculateMoveVal()
{
	mMoveVal += static_cast<int>(1000.0f / GetSpeed());
}

int Character::MaxHealth()
{
	if (Student* stu = dynamic_cast<Student*>(this))
		return stu->student->maxfocus;
	else if (Teacher* tea = dynamic_cast<Teacher*>(this))
		return tea->teacher->maxhomework;
	else return 0;
}

int Character::GetHealth() const { return cur_health; }

void Character::SetHealth(int h)
{
	cur_health = h;
	if (cur_health > MaxHealth()) cur_health = MaxHealth();
}

void Character::TakeDamage(int dmg)
{
	cur_health -= dmg;
	if (cur_health < 0) cur_health = 0;
}

bool Character::IsDead() const { return cur_health <= 0; }

float Character::GetStrength() const { return strength; }

Student::Student(const _Character& char_data)
{
	student = &g_Students.at(char_data.name);
	strength *= char_data.star;
	if (char_data.extra.empty()) return;
	mEquipment = new Card(char_data.extra);
}

Teacher::Teacher(const _Character& char_data)
{
	teacher = &g_Teachers.at(char_data.name);
	strength *= char_data.star;
	if (char_data.extra.empty()) return;
	strength *= std::stoi(char_data.extra);
}

DYC_END