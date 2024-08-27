#pragma once
#include <QObject>
#include <QList>

#include <QAbstractListModel>

class MetaInfo;

class MetaModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum ModelRoles {
		IdRole = Qt::UserRole + 1,
		RidRole,
		TypeRole,
		TitleRole,
		NickRole,
		AvatarRole,
		SnapshotRole,
		CategoryRole,
		FollowRole,
		FavRole,
		HeatRole,
		LiveRole,
		StartTimeRole,
		LastUpdateRole,
	};
	MetaModel(QList<MetaInfo>& source, QObject *parent = 0);
	~MetaModel();

public:
	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

public:
	void changeAtIndex(int idx);
	void reset();
	void update(const MetaInfo& mi);
	using Updater = std::function<void(MetaInfo&)>;
	bool update(const QString& type, const QString& rid, Updater&& up);
	void remove(const QString& type, const QString& rid);

private:
	QList<MetaInfo>& source_;
};
