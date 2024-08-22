#pragma once
#include <QObject>
#include <QList>

#include <QAbstractListModel>

class MetaInfo;
class MetaModel;

class Model : public QObject
{
	Q_OBJECT
public:
	explicit Model(QObject *parent = 0);
	~Model();

	void initSchema();

	void loadFollows();
	void updateFollow(MetaInfo*);
	// QList<MetaInfo*> outdated(int gap);

	MetaModel* data();

signals:

private:
	MetaModel* model_;
};

class MetaModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum MetaRoles {
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
	MetaModel(QObject* parent = nullptr);

	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

public:
	void append(MetaInfo *mi);
	void update(MetaInfo* mi);

private:
	QList<MetaInfo*> follows_;
};
