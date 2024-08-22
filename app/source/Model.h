#pragma once
#include <QObject>
#include <QList>

#include <QAbstractListModel>

class MetaInfo;
class MetaModel;

class Model : public QAbstractListModel
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
	explicit Model(QObject *parent = 0);
	~Model();

	void initSchema();

	void loadFollows();
	void updateFollow(const MetaInfo&);
	QList<std::pair<QString, QString>> outdated(int duration);


public:
	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;


private:
	void doLoad(const MetaInfo& mi);
	void doUpdate(const MetaInfo& mi);

signals:

private:
	QList<MetaInfo> follows_;
};
