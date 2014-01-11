/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LDB_QUERY_H_
#define LDB_QUERY_H_

#include <efc/List>
#include <liquiddb/Field>


namespace LiquidDb {

class Join;
class Constraint;
class Database;
class Entity;


class Query
{
public:
	typedef ::EFC::List<Field>              Fields;
	typedef ::EFC::List<const Join *>       Joins;
	typedef ::EFC::List<const Constraint *> Constraints;

	struct Value
	{
		const Table::Column *column;
		const void *value;
		size_t size;
	};
	typedef ::EFC::List<Value> Values;

public:
	Query();
	virtual ~Query();

	void join(const Join &link);
	void where(const Constraint &constraint);

	virtual int build(char *buffer, size_t size) const = 0;

protected:
	Joins m_joins;
	Constraints m_constraints;
};


class Select : public Query
{
public:
	Select(const Table &from);
	virtual ~Select();

	void select(const Table &table);
	void select(const Table &table, const Table::Column *column);
	virtual int build(char *buffer, size_t size) const;

private:
	friend class Database;
	Fields m_fields;

private:
	const Table *m_from;
};


class Insert : public Query
{
public:
	Insert(const Table &into);
	virtual ~Insert();

	void insert(const Table::Column *column, const void *value);
	void insert(const Table::Column *column, const void *value, size_t size);
	virtual int build(char *buffer, size_t size) const;

private:
	Values m_values;
	const Table *m_into;
};


class Update : public Query
{
public:
	Update(const Table &table);
	virtual ~Update();

	void update(const Table::Column *column, const void *value);
	void update(const Table::Column *column, const void *value, size_t size);
	virtual int build(char *buffer, size_t size) const;

private:
	Values m_values;
	const Table *m_table;
};


class Delete : public Query
{
public:
    Delete(const Table &from);
    virtual ~Delete();

    virtual int build(char *buffer, size_t size) const;

private:
    const Table *m_from;
};


class SelectEntity : public Query
{
public:
    SelectEntity(const Entity &entity);
    virtual ~SelectEntity();

    virtual int build(char *buffer, size_t size) const;

private:
    using Query::join;

private:
    friend class Database;
    Fields m_fields;

private:
    const Entity &m_entity;
};

}

#endif /* LDB_QUERY_H_ */