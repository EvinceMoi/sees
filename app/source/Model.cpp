#include "Model.h"
#include <QModelIndex>
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


// void Model::doUpdate(const MetaInfo& mi)
// {
// 	QList<int> roles = {
// 		TitleRole,
// 		NickRole,
// 		AvatarRole,
// 		SnapshotRole,
// 		CategoryRole,
// 		FollowRole,
// 		FavRole,
// 		HeatRole,
// 		LiveRole,
// 		StartTimeRole
// 	};
// 	auto it = std::find_if(follows_.begin(), follows_.end(), [mi](auto& it) {
// 		return it.type == mi.type && it.rid == mi.rid;
// 	});
// 	if (it == follows_.end()) {
// 		follows_.append(mi);
// 		follows_.last().follow = true;
// 		auto idx = createIndex(follows_.size() - 1, 0);
// 		emit dataChanged(idx, idx, roles);
// 	} else {
// 		auto id = it->id;
// 		*it = mi;
// 		it->id = id;
// 		it->lastUpdate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
// 		qDebug() << "last update:" << it->lastUpdate;
// 		auto idx = createIndex(std::distance(follows_.begin(), it), 0);
// 		emit dataChanged(idx, idx, roles);
// 	}
// 	qDebug() << "update rid:" << mi.rid << ", type:" << mi.type;
// }
