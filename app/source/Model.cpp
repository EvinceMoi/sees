#include "Model.h"
#include <QModelIndex>
#include <QDateTime>
#include <QDebug>

#include "Types.h"

MetaModel::MetaModel(QList<MetaInfo>& source, QObject *parent)
	: QAbstractListModel(parent)
	, source_(source)
{
}

MetaModel::~MetaModel()
{
}

QHash<int, QByteArray> MetaModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[IdRole] = "id";
	roles[RidRole] = "rid";
	roles[TypeRole] = "type";
	roles[TitleRole] = "title";
	roles[NickRole] = "nick";
	roles[AvatarRole] = "avatar";
	roles[SnapshotRole] = "snapshot";
	roles[CategoryRole] = "category";
	roles[FollowRole] = "follow";
	roles[FavRole] = "fav";
	roles[HeatRole] = "heat";
	roles[LiveRole] = "live";
	roles[StartTimeRole] = "startTime";
	roles[LastUpdateRole] = "lastUpdate";
	return roles;
}

QVariant MetaModel::data(const QModelIndex &idx, int role) const
{
	if (!idx.isValid()) return {};
	auto n = idx.row();
	if (n >= source_.size()) return {};
	auto it = source_[n];
	switch (role) {
		case IdRole: return it.id;
		case RidRole: return it.rid;
		case TypeRole: return it.type;
		case TitleRole: return it.title;
		case NickRole: return it.nick;
		case AvatarRole: return it.avatar;
		case SnapshotRole: return it.snapshot;
		case CategoryRole: return it.category;
		case FollowRole: return it.follow;
		case FavRole: return it.fav;
		case HeatRole: return it.heat;
		case LiveRole: return it.live;
		case StartTimeRole: return it.startTime;
		case LastUpdateRole: return it.lastUpdate;
	}
	return {};
}

int MetaModel::rowCount(const QModelIndex &) const
{
	return source_.size();
}

void MetaModel::changeAtIndex(int idx)
{
	QList<int> roles = {
		TitleRole,
		NickRole,
		AvatarRole,
		SnapshotRole,
		CategoryRole,
		FollowRole,
		FavRole,
		HeatRole,
		LiveRole,
		StartTimeRole
	};

	auto index = createIndex(idx, 0);
	emit dataChanged(index, index, roles);
}

void MetaModel::reset()
{
	beginResetModel();
	endResetModel();
}

void MetaModel::update(const MetaInfo &mi)
{
	auto it = std::find_if(source_.begin(), source_.end(), [mi](auto& it) {
		return it.type == mi.type && it.rid == mi.rid;
	});
	if (it == source_.end()) {
		auto size = source_.size();
		beginInsertRows(QModelIndex(), size, size);
		source_.append(mi);
		endInsertRows();
	} else {
		auto idx = std::distance(source_.begin(), it);
		*it = mi;
		changeAtIndex(idx);
	}
}

bool MetaModel::update(const QString &type, const QString &rid, Updater &&up)
{
	auto it = std::find_if(source_.begin(), source_.end(), [type, rid](auto& it) {
		return it.type == type && it.rid == rid;
	});
	if (it == source_.end())
		return false;

	auto idx = std::distance(source_.begin(), it);
	up(*it);
	changeAtIndex(idx);
	return true;
}

void MetaModel::remove(const QString &type, const QString &rid)
{
	beginResetModel();
	source_.erase(std::remove_if(source_.begin(), source_.end(), [type, rid](auto mi){
		return type == mi.type && rid == mi.rid;
	}), source_.end());
	endResetModel();
}
